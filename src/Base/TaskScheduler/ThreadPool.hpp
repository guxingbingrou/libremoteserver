#ifndef _TASKSCHEDULER_THREADPOOL_H_
#define _TASKSCHEDULER_THREADPOOL_H_
#include <memory>
#include <thread>
#include <queue>
#include <future>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <atomic>

class ThreadPool
{
private:
    std::vector<std::thread> m_wokers;  //woker threads
    std::queue<std::function<void()>> m_tasks; // tasks

    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::atomic_bool m_running{true};

public:
    ThreadPool(int size){

        for(int i=0; i<size; ++i){
            m_wokers.emplace_back([this]{
                while(this->m_running){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lck(this->m_mutex);
                        this->m_condition.wait(lck, [this]{
                            return !this->m_running || !this->m_tasks.empty();
                        });
                        if(!this->m_running && this->m_tasks.empty())
                            return;

                        task = std::move(this->m_tasks.front());
                        this->m_tasks.pop();
                    }

                    task();
                }
            });
        }
    }
    template<typename F, typename... Args>
    auto CommitTask(F&& f, Args&&... args) -> std::future<decltype(f(args...)) >{
        if(!m_running)
            throw std::runtime_error("threadpool is not running");

        using ReturnType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<ReturnType()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<ReturnType> ret = task->get_future();
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            m_tasks.emplace([task]{
                (*task)();
            });
        }
        m_condition.notify_one();
        return ret;
    }

    ~ThreadPool(){
        m_running = false;
        m_condition.notify_all();
        for(auto& worker : m_wokers){
            if(worker.joinable())
                worker.join();
        }
    }

};





#endif