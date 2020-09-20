/**
 * 实现了对象池s
 */
#pragma once

#include <list>
#include <mutex>

template<class Object>
class ObjectPool {
private:
    std::list<Object> objs;
    std::unique_lock<std::mutex> mutex;
public:
    Object Get() {
        mutex.lock();
        if (objs.empty()) {
            mutex.unlock();
            return Object();
        }
        Object data = *(objs.end());
        objs.pop_back();
        mutex.unlock();
        return data;
    }

    void Put(Object obj) {
        mutex.lock();
        objs.push_back(obj);
        mutex.unlock();
        return;
    }
};