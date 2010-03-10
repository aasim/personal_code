#ifndef MY_VECTOR__HPP__
#define MY_VECTOR__HPP__

#include <vector>
#include <assert.h>


template <typename T>
class Data {
public:
    Data(): count(1){
    }

    Data(const Data & _data): count(1) {  
        list = _data.list;
    }

    const int & at(size_t index) {
        return list.at(index);
    }

    void push_back(const T & _elem) {
        list.push_back(_elem);
    }
    
    size_t count;

private:
    std::vector<T> list;
};


template <typename T>
class MyVector {
public:
    MyVector(): data(new Data<T>()) { };
    
    MyVector(const MyVector & _right): data(_right.data) {
        ++data->count;
    };

    ~MyVector() {
        if(--data->count == 0) delete data;
    }

    MyVector & operator = (MyVector & _right) {
        Data<T> * const old = data;
        data = _right.data;
        ++data->count;
        if(--old->count == 0) delete old;
        return *this;
    }

    const T & at(size_t index) const {
        return data->at(index);
    }

    void push_back(const T & _elem) {
        if(data->count > 1) {
            Data<T> * _temp = new Data<T>(*data);
            --data->count;
            data = _temp;
        }

        assert(data->count == 1);

        data->push_back(_elem);
    }

    const Data<T> * get_data() const { return data; }

private:
    Data<T> * data;     
};


#endif
