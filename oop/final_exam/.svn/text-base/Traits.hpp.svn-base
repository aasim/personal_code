#ifndef TRAITS__HPP__
#define TRAITS__HPP__

template <typename T> struct Traits; 

template <> struct Traits<int> {
    static const char * format() {
        return "%d";
    }
}; 

template <> struct Traits<double> { 
    static const char * format() {
        return "%g";
    }
};

template <typename T>
void 
output(char *buf, const T &v2) {
    sprintf(buf, Traits<T>::format(), v2);
}

#endif
