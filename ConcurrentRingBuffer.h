//
// Created by Shinan on 2019/1/15.
//

#ifndef SEVICEFRAMEWORK_CONCURRENTRINGBUFFER_H
#define SEVICEFRAMEWORK_CONCURRENTRINGBUFFER_H

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <iostream>

template <class T>
class ConcurrentRingBuffer{
public:
    ConcurrentRingBuffer(size_t size)
    :size_(size)
    ,buff_(nullptr)
    ,head_(0)
    ,tail_(0)
    {
        buff_ = new T[size];
    }

    ~ConcurrentRingBuffer() {
        if(buff_ != nullptr){
            delete[] buff_;
            buff_ = nullptr;
        }
    }

    void syncPut(const T &obj) {
        std::unique_lock<std::mutex> lck(mtx_);
        while(_next(head_) == tail_){
            condNotFull_.wait(lck);
        }
        buff_[head_] = obj;
        head_ = _next(head_);
        condNotEmpty_.notify_one();
    }

    bool asyncPut(const T &obj) {
        std::unique_lock<std::mutex> lck(mtx_);
        if(_next(head_) == tail_)
            return false;
        buff_[head_] = obj;
        head_ = _next(head_);
        condNotEmpty_.notify_one();
    }

    bool syncGet(T &obj) {
        std::unique_lock<std::mutex> lck(mtx_);
        while(tail_ == head_){
            condNotEmpty_.wait(lck);
        }
        obj = buff_[tail_];
        tail_ = _next(tail_);
        condNotFull_.notify_one();
        return true;
    }

    bool asyncGet(T &obj) {
        std::unique_lock<std::mutex> lck(mtx_);
        if(tail_ == head_)
            return false;
        obj = buff_[tail_];
        tail_ = _next(tail_);
        condNotFull_.notify_one();
        return true;
    }

private:
    inline size_t _next(size_t index){
        return (index + 1) % size_;
    }
private:
    T* buff_;
    size_t size_;
    size_t head_;
    size_t tail_;
    std::mutex mtx_;
    std::condition_variable condNotFull_;
    std::condition_variable condNotEmpty_;
};

#endif //SEVICEFRAMEWORK_CONCURRENTRINGBUFFER_H
