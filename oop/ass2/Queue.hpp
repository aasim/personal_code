#ifndef ASS2_QUEUE_HPP
#define ASS2_QUEUE_HPP

#include <assert.h>

namespace ass2 {
    template <typename T>
    class Queue {
        public:
            Queue();
            Queue(const Queue &);
            ~Queue();
            void push_back(const T &);
            T & front();
            void pop_front();

        private:
            T * array;
            size_t first, next;
            size_t count;
            size_t size;

    };

    template <typename T>
    void
    destroy(T * _a, size_t _n) {
        for(size_t i = 0; i < _n; i++) {
            _a[i].~T();
        }
    }

    template <typename T>
    void
    destroy(T * _a, size_t _first, size_t _count, size_t _size) {
        while (_count--) {
            _a[_first].~T();
            _first = (_first + 1) % _size;
        }
    }


    template <typename T>
    T *
    new_copy(const T *src, size_t src_size, size_t dst_size, size_t next, size_t first) {
        assert(dst_size >= src_size);

        T * dst = static_cast<T *>(::operator new(dst_size * sizeof(T)));
        size_t j = 0;
        size_t i = first;
        try {
            for(; i < src_size; i++, j++) {
                new (dst + j) T (src[i]);
            }
            
            for(i=0; i < next; j++, i++) {
                new (dst + j) T(src[i]);
            }
        } catch (...) {
            destroy(dst, j);
            ::operator delete(dst);
            throw;
        }

        return dst;
    }

    template <typename T>
    T *
    new_copy(const T *src, size_t src_size, size_t dst_size) {
        assert(dst_size >= src_size);

        T * dst = static_cast<T *>(::operator new(dst_size * sizeof(T)));
        size_t i = 0;
        try {
            for(; i < src_size; i++) {
                new (dst + i) T (src[i]);
            }
        } catch (...) {
            destroy(dst, i);
            ::operator delete(dst);
            throw;
        }

        return dst;
    }

    template <typename T>
    Queue<T>::Queue():
    size(10), count(0), first(0), next(0) {
        array = static_cast<T *>(::operator new(sizeof(T) * size));
    }

    template <typename T>
    Queue<T>::Queue(const Queue & _src):
    array(new_copy(_src.array, _src.size, _src.size)), size(_src.size), first(_src.first), next(_src.next), count(_src.count) {
    }

    template <typename T>
    Queue<T>::~Queue() {
        destroy(array, first, count, size);
        ::operator delete (array);
    }

    template <typename T>
    void
    Queue<T>::push_back(const T & elem) {
        if(count == size) {
           size_t new_size = size * 2;
           T * new_array = new_copy(array, size, new_size, next, first);
           destroy(array, first, count, size);
           ::operator delete (array);
           array = new_array;
           next = size;
           first = 0;
           size = new_size;
        }
        new (array + next) T(elem);
        next = (next + 1) % size;
        count ++;
    }

    template <typename T>
    T &
    Queue<T>::front() {
        if(count == 0)
            throw "Empty";
        return array[first];
    }


    template <typename T>
    void
    Queue<T>::pop_front() {
        if(count == 0)
            throw "Empty";
        array[first].~T();
        first = (first + 1) % size;
        
        count --;
    }
}
#endif
