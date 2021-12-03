#ifndef QUEUE_H
#define QUEUE_H

#ifndef NULL
#define NULL    0
#endif

template<class T> class Queue
{
public:
    int front=0;
    int rear=0;
    int size=100; //Free to define (recommand 100 or 200)
    T *val;

    Queue(){
        val = new T[size];
        //Needs extra init
    }
    ~Queue()
    {
        delete[] val;
    }

    void push(T value) {
        val[rear++]=value;
    }

    void pop() {
        if(!empty()) front++;
    }

    bool empty() {
        return front==rear;
    }

    bool isFull() {
        //Check queue is full or not
    }

    void clear(){
        front=rear=0;
    }
};


#endif
