/*
 * BufferQueue.h
 *
 *  Created on: Dec 28, 2020
 *      Author: zou
 */

#ifndef SRC_BASE_BUFFERQUEUE_BUFFERQUEUE_HPP_
#define SRC_BASE_BUFFERQUEUE_BUFFERQUEUE_HPP_

#include <queue>
#include <vector>
#include <mutex>
#include <chrono>
#include <condition_variable>


namespace BufferQueue{

template <class T>
class BufferQueue {
public:
	BufferQueue(int size = 5){m_size = size;};
	inline bool IsEmpty() const{return m_buffer_queue.empty();}
	inline bool IsFull() const{return m_buffer_queue.size() >= m_size;}
	inline bool IsRunning() const{return m_running;}
	void WaitePopBuffer(std::vector<T>& buffer){
		std::unique_lock<std::mutex> lck(m_mutex);
		
		while(IsEmpty() && m_running){
			m_condition_pop.wait(lck, [this]{return !this->IsEmpty() | !this->IsRunning();});
		}
		if(!IsEmpty()){
			buffer = m_buffer_queue.front();
			m_buffer_queue.pop();
			m_condition_push.notify_one();
		}
	};
	void WaitePushBuffer(std::vector<T>& buffer){
		std::unique_lock<std::mutex> lck(m_mutex);

		while(IsFull() && m_running){
			m_condition_push.wait(lck, [this]{return !this->IsFull() | !this->IsRunning();});
		}
		if(!IsFull()){
			m_buffer_queue.push(std::move(buffer));
			m_condition_pop.notify_one();
		}

	};
	void Start(){m_running = true;}
	void Stop(){
		m_running = false;
		m_condition_push.notify_all();
		m_condition_pop.notify_all();
	}
	virtual ~BufferQueue(){Stop();};

private:
	size_t m_size = 0;
	std::queue<std::vector<T>> m_buffer_queue;
	std::mutex m_mutex;
	bool m_running = true;
	std::condition_variable m_condition_push;
	std::condition_variable m_condition_pop;

};

}/* namespace RemoteServer */
#endif /* SRC_BASE_BUFFERQUEUE_BUFFERQUEUE_HPP_ */
