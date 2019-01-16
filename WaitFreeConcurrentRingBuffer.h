//
// Created by Shinan on 2019/1/15.
//wait-free ring buffer just for single consumer and single producer

#ifndef SEVICEFRAMEWORK_WAITFREECONCURRENTRINGBUFFER_H
#define SEVICEFRAMEWORK_WAITFREECONCURRENTRINGBUFFER_H

#include <atomic>

template <class T>
class WaitFreeConcurrentRingBuffer{
public:
    WaitFreeConcurrentRingBuffer(size_t size)
    :size_(size)
    ,buff_(nullptr)
    ,head_(0)
    ,tail_(0)
    {
        buff_ = new T[size];
    }

    ~WaitFreeConcurrentRingBuffer() {
        if(buff_ != nullptr){
            delete[] buff_;
            buff_ = nullptr;
        }
    }

    bool asyncPut(const T &obj) {
        size_t head = head_.load(std::memory_order_relaxed);
        size_t tail = tail_.load(std::memory_order_acquire);
        size_t nextIdx = _next(head);
        if( nextIdx == tail)
            return false;
        buff_[head] = obj;
        head_.store(nextIdx, std::memory_order_release);
        return true;
    }

    bool asyncGet(T &obj) {
        size_t tail = tail_.load(std::memory_order_relaxed);
        size_t head = head_.load(std::memory_order_acquire);
        if(tail == head)
            return false;
        obj = buff_[tail];
        tail_.store(_next(tail), std::memory_order_release);
        return true;
    }

private:
    inline size_t _next(size_t index){
        return (index + 1) % size_;
    }
    WaitFreeConcurrentRingBuffer();
private:
    T* buff_;
    size_t size_;
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;
};



#endif //SEVICEFRAMEWORK_WAITFREECONCURRENTRINGBUFFER_H
