//
// Created by xiucheng yin on 2018/10/26.
//

#ifndef TINAPLAYER_SAFE_QUEUE_H
#define TINAPLAYER_SAFE_QUEUE_H

#include <queue>
#include <pthread.h>

using namespace std;

template<typename T>

class SafeQueue {

    //定义一个函数指针
    typedef void(*RelaseCallback)(T *);

    typedef void (*SyncHandle)(queue<T> &);

public:
    SafeQueue() {
        pthread_mutex_init(&mutex, 0);
        pthread_cond_init(&cond, 0);
    }

    ~SafeQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    void push(T new_value) {
        pthread_mutex_lock(&mutex);
        if (work) {
            q.push(new_value);
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
        } else {//没有释放，内存泄漏
            relaseCallback(&new_value);
        }
        pthread_mutex_unlock(&mutex);
    }

    int pop(T &value) {
        int ret = 0;
        pthread_mutex_lock(&mutex);
        while (work && q.empty()) {
            //没有数据就等待
            pthread_cond_wait(&cond, &mutex);
        }
        if (!q.empty()) {
            value = q.front();
            q.pop();
            ret = 1;
        }
        pthread_mutex_unlock(&mutex);
        return ret;
    }

    void setWork(int work) {
        pthread_mutex_lock(&mutex);
        this->work = work;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    int empty() {
        return q.empty();
    }

    int size() {
        return q.size();
    }

    void clear() {
        pthread_mutex_lock(&mutex);
        int size = q.size();
        for (int i = 0; i < size; ++i) {
            //取出队首的变量
            T value = q.front();

            //释放value
            if (relaseCallback) {
                relaseCallback(&value);
            }

            q.pop();
        }
        pthread_mutex_unlock(&mutex);
    }

    void sync() {
        pthread_mutex_lock(&mutex);
        //同步代码
        syncHandle(q);
        pthread_mutex_unlock(&mutex);
    }

    void setSyncHandle(SyncHandle s) {
        syncHandle = s;
    }

    void setRelaseCallback(RelaseCallback relaseCallback) {
        this->relaseCallback = relaseCallback;
    }

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    queue<T> q;
    int work;

    RelaseCallback relaseCallback;

    SyncHandle syncHandle;
};

#endif //TINAPLAYER_SAFE_QUEUE_H
