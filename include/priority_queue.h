#ifndef PRIORITY_QUEUE_H
#define RIORITY_QUEUE_H

#include "list.hpp"
template <typename T>
class priority_queue {
    uj::list<T> array;
    unsigned int currentsize;

    void repair(unsigned int x) {
        int child;
        T tmp = array[x];
        for(; x*2 <= currentsize; x = child) {
            child = x * 2;
            if(x != currentsize && array[child+1] < array[child])
                child++;
            if(array[child] < tmp)
                array[x] = array[child];
            else
                break;
        }
        array[x] = tmp;
    }

public:
    priority_queue() : currentsize(0) {
        array.resize(100);
    }
    bool empty() const { return !currentsize; }
    unsigned int size() const { return currentsize; }
    T top() {
        return array[1];
    }

    void push(const T& x) {
        if(currentsize == array.size() - 1) {
            array.resize(array.size() * 2);
        }
        int y = ++ currentsize;
        for(; y > 1 && x < array[y/2]; y /= 2) {
            array[y] = array[y/2];
        }
        array[y] = x;
    }

    void pop() {
        if(empty())
            return;
        array[1] = array[currentsize--];
        repair(1);
    }
};

#endif //PRIORITY_QUEUE_H
