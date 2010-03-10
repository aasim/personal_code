#ifndef ASS2_ARRAY2_HPP
#define ASS2_ARRAY2_HPP
#include <iostream>

class OutOfRange { };

namespace ass2 {

    template <typename T>
    class Array2 {
        public:
            Array2(int _rows, int _cols): rows(_rows), cols(_cols) {
                mem = new T[rows * cols];
            }
    
            ~Array2() { delete []  mem;  }
    
            T & operator () (int _row, int _col) {
                return *(mem + (_row * cols + _col));
            }
    
            const T & operator () (int _row, int _col) const {
                return *(mem + (_row * cols + _col));
            }
    
            class Array2Col {
                public:
                    Array2Col(int _row, int _cols, Array2<T> & _array2): row(_row), cols(_cols), array2(_array2) { }
                    T & operator [] (int _col) {
                        if(_col < 0 || _col >= cols) throw OutOfRange();
                        return array2(row, _col);
                    }
                private:
                    int row;
                    int cols;
                    Array2<T> & array2;
            };
            
            class Array2ColConst {
                public:
                    Array2ColConst(int _row, int _cols, const Array2<T> & _array2): row(_row), cols(_cols), array2(_array2) { }
                    const T & operator [] (int _col) const{
                        if(_col < 0 || _col >= cols) throw OutOfRange();
                        return array2(row, _col);
                    }
                private:
                    int row;
                    int cols;
                    const Array2<T> & array2;
            };
    
    
            Array2Col operator [] (int _row) {
                if(_row < 0 || _row >= rows) throw OutOfRange();
                return Array2Col(_row, cols, *this);
            }
    
            Array2ColConst operator [] (int _row) const {
                if(_row < 0 || _row >= rows) throw OutOfRange();
                return Array2ColConst(_row, cols, *this);
            }
    
        private:
            int rows;
            int cols;
            T * mem;
    };
    
    template <>
    class Array2 <bool> {
        public:
            Array2(int _row, int _col) : rows(_row), cols(_col) {
                if(((rows * cols) % bits_per_byte) == 0)
                    mem = new char[((rows * cols) / bits_per_byte)];
                else
                    mem = new char[((rows * cols) / bits_per_byte) + 1];
            }
    
            ~Array2() { delete [] mem; }
           
            friend class BitToBool; 
    
            class BitToBool {
                public:
                    BitToBool(Array2<bool> & _array, int _byte_index, int _bit_mask): 
                        array(_array), byte_index(_byte_index), bit_mask(_bit_mask) { }
                    BitToBool & operator = (const bool & _value) {
                        if(_value)
                            *(array.mem + byte_index) |= bit_mask;
                        else {
                            bit_mask = ~bit_mask;
                            *(array.mem + byte_index) &= bit_mask;
                        }
                        return *this;
                    } 
                    
                    BitToBool & operator = (const BitToBool & _value) {
                        if(*this == _value) return *this;
                        if(_value)
                            *(array.mem + byte_index) |= bit_mask;
                        else {
                            bit_mask = ~bit_mask;
                            *(array.mem + byte_index) &= bit_mask;
                        }
                        return *this;
                    }
    
                    operator bool () const {
                        return (*(array.mem + byte_index) & bit_mask) != 0;
                        
                    }
    
                private:
                    const int byte_index;
                    char bit_mask;
                    Array2<bool> & array;
            };
           
            BitToBool operator () (int _row, int _col) {
                int index = _row * cols + _col;
                int byte_index = index / bits_per_byte;
                int bit_index = index - byte_index * bits_per_byte;
                const char bit_mask = 1 << bit_index;
                return BitToBool(*this, byte_index, bit_mask);
            }
            
            const bool operator () (int _row, int _col) const {
                int index = _row * cols + _col;
                int byte_index = index / bits_per_byte;
                int bit_index = index - byte_index * bits_per_byte;
                const char bit_mask = 1 << bit_index;
                return (*(mem + byte_index) & bit_mask) != 0;
            }
    
            class Array2Col {
                public:
                    Array2Col(int _row, int _cols, Array2<bool> & _array2): row(_row), cols(_cols), array2(_array2) { }
                    BitToBool operator [] (int _col) {
                        if(_col < 0 || _col >= cols) throw OutOfRange();
                        return array2(row, _col);
                    }
    
                private:
                    int row;
                    int cols;
                    Array2<bool> & array2;
            };
            
            class Array2ColConst {
                public:
                    Array2ColConst(int _row, int _cols, const Array2<bool> & _array2): row(_row), cols(_cols), array2(_array2) { }
                    const bool operator [] (int _col) const {
                        if(_col < 0 || _col >= cols) throw OutOfRange();
                        return array2(row, _col);
                    }
    
                private:
                    int row;
                    int cols;
                    const Array2<bool> & array2;
            };
    
    
            Array2Col operator [] (int _row) {
                if(_row < 0 || _row >= rows) throw OutOfRange();
                return Array2Col(_row, cols, *this);
            }
            
            const Array2ColConst operator [] (int _row) const {
                if(_row < 0 || _row >= rows) throw OutOfRange();
                return Array2ColConst(_row, cols, *this);
            }
    
    
    #ifdef TEST
            bool is_Bool() { return true; }
    #endif
    
        private:
            int rows;
            int cols;
            const static int bits_per_byte = 8;
            char * mem;
    };
}    
#endif
