//
// Created by xu on 21-7-28.
//

// 模板class 的声明和实现不能拆分到.h和.cpp两个文件
#ifndef MYMENGMEDIA_THREADSAFEQUEUE_H
#define MYMENGMEDIA_THREADSAFEQUEUE_H

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue{
public:
    ThreadSafeQueue(){}
    ThreadSafeQueue(ThreadSafeQueue const &other){
        std::lock_guard<std::mutex> lock(other.m_mutex);
        m_data_queue = other.m_data_queue;
    };
    void push(T t){
        std::lock_guard<std::mutex>lock(m_mutex);
        m_data_queue.push(t);
        m_condition_var.notify_one();
    };
    T pop(){
        std::lock_guard<std::mutex>lock(m_mutex);
        if(isEmpty())return nullptr;
        T t = m_data_queue.front();
        m_data_queue.pop();
        return t;
    };
    bool isEmpty(){
        return m_data_queue.empty();
    };
    int size(){
        std::lock_guard<std::mutex>lock(m_mutex);
        return m_data_queue.size();
    };

private:
    mutable std::mutex m_mutex;
    std::queue<T> m_data_queue;
    std::condition_variable m_condition_var;
};
#endif //MYMENGMEDIA_THREADSAFEQUEUE_H
