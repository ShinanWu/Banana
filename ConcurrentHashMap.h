//
// Created by Shinan on 2019/1/20.
//
//增强型并发的哈希表（简易封装，对此project够用），哈希锁减少锁的碰撞，注意设置哈希锁的数目HASHLOCKS
//HASHLOCKS值越大性能越好，但是内存消耗也越多，建议设置和IndependentTask数目相当即可
//
#ifndef SEVICEFRAMEWORK_CONCURRENTHASHMAP_H
#define SEVICEFRAMEWORK_CONCURRENTHASHMAP_H

#include <unordered_map>
#include <mutex>

#define DEFAULT_HASHLOCKS 499 //选择质数
using namespace std;

template <class K, class V, size_t HASHLOCKS=DEFAULT_HASHLOCKS>
class ConcurrentHashMap{
public:
    bool find(K k, V& v) {
        size_t bucketIdx = hasher_(k) % HASHLOCKS;
        unique_lock<mutex> lck(mtx_[bucketIdx]); //减少锁的碰撞
        auto it = map_[bucketIdx].find(k);
        if(it != map_[bucketIdx].end()){
            v = it->second;
            return true;
        }
        return false;
    }

    bool insert(K k, const V& v) {
        size_t bucketIdx = hasher_(k) % HASHLOCKS;
        unique_lock<mutex> lck(mtx_[bucketIdx]);
        auto pairKV = map_[bucketIdx].insert(make_pair(k, v));
        if(pairKV.second == true){
            return true;
        }
        return false;
    }
    void erase(K k){
        size_t bucketIdx = hasher_(k) % HASHLOCKS;
        unique_lock<mutex> lck(mtx_[bucketIdx]);
        map_[bucketIdx].erase(k);
    }

private:
    mutex mtx_[HASHLOCKS];
    unordered_map<K, V> map_[HASHLOCKS];
    hash<K> hasher_;

};

#endif //SEVICEFRAMEWORK_CONCURRENTHASHMAP_H
