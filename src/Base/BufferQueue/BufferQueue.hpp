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
	bool IsEmpty(){return m_buffer_queue.empty();}
	bool IsFull(){return m_buffer_queue.size() >= m_size;}
	void WaitePopBuffer(std::vector<T>& buffer){
		std::unique_lock<std::mutex> lck(m_mutex);
		while(IsEmpty() && m_waite_pop){
			m_condition_pop.wait_for(lck, std::chrono::seconds(1));
		}
		if(!IsEmpty()){
			buffer = m_buffer_queue.front();
			m_buffer_queue.pop();
			m_condition_push.notify_one();
		}
	};
	void WaitePushBuffer(std::vector<T>& buffer){
		std::unique_lock<std::mutex> lck(m_mutex);
		while(IsFull() && m_waite_push){
			m_condition_push.wait_for(lck, std::chrono::seconds(1));
		}
		if(!IsFull()){
			m_buffer_queue.push(std::move(buffer));
			m_condition_pop.notify_one();
		}

	};
	void SetWaitePush(bool waite_push){m_waite_push = waite_push;};
	void SetWaitePop(bool wait_pop){m_waite_pop = wait_pop;};
	virtual ~BufferQueue(){};

private:
	size_t m_size = 0;
	std::queue<std::vector<T>> m_buffer_queue;
	std::mutex m_mutex;
	bool m_waite_push = true;
	bool m_waite_pop = true;
	std::condition_variable m_condition_push;
	std::condition_variable m_condition_pop;

};

}/* namespace RemoteServer */
#endif /* SRC_BASE_BUFFERQUEUE_BUFFERQUEUE_HPP_ */
