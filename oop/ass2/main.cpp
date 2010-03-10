#include "Array2.hpp"
#include "Queue.hpp"
#include <iostream>
#include <assert.h>
#include <string.h>

using namespace ass2;

#ifdef TEST

class NoEqualOperator {
    public:
        NoEqualOperator(int _i): i(_i) { }
        int value() const { return i; }
    private:
        int i;
};

class Node {
    public:
        Node() : i(0), j(0) { }
        Node(int _i, int _j): i(_i), j(_j) { }
        Node & operator = (const Node & _node) {
            if(this == &_node) return *this;

            i = _node.get_i();
            j = _node.get_j();

            return *this;
        }
        Node(const Node & _node) {
            if(this == &_node) return;
            i = _node.get_i();
            j = _node.get_j();
        }

        int get_i() const { return i; }
        int get_j() const { return j; }
    private:
        int i;
        int j;
};

template <typename T>
void queue_test(Queue<T> q) {
    for(int i=0; i < 50000; i++) {
        assert(q.front() == 1);
        q.pop_front();
    }
    for(int i=0; i < 100000; i++) {
        assert(q.front() == 2);
        q.pop_front();
    }
    return;
}

template <typename T>
void check_indexing(Array2<T> & _array, int _i, int _j) {
    try {
        std::cout << _array[_i][_j];
    } catch (OutOfRange) {
        assert(true);
        return;
    } catch (...) {
        std::cout << "Checking indexes for " << _i << ", " << _j << std::endl;
        assert(false);
    }
    std::cout << "Checking indexes for " << _i << ", " << _j << std::endl;
    assert(false);
}

bool test() {
    // Test creation of array of different types
    try {
        Array2<int> int_array(5, 5);
        Array2<float> float_array(5, 5);
        Array2<char> char_array(5, 5);
        Array2<char *> char_ptr_array(5, 5);
        Array2<int *> int_ptr_array(5, 5);
        Array2<Node> node_array(5, 5);
        Array2<Node *> node_ptr_array(5, 5);
    } catch (...) {
        std::cout << "Creating different array types failed" << std::endl;
        return false;
    }

    // Check () operator by entering values for int
    try {
        Array2<int> array(4, 5);
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 5; j++) {
                array(i, j) = array(i, j) = i * j;
            }
        }
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 5; j++) {
                assert(array(i, j) == (i * j));
            }
        }
    } catch (...) {
        std::cout << "Checking () operator failed" << std::endl;
        return false;
    }

    // Check [] operator by entering values for float
    try {
        Array2<float> array(4, 5);
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 5; j++) {
                array[i][j] = array[i][j] = (float)i * j;
            }
        }
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 5; j++) {
                assert(array[i][j] == (i * j));
            }
        }
    } catch (...) {
        std::cout << "Checking [] operator failed" << std::endl;
        return false;
    }

    // Check if [] operator throws if out of index operation is done for char
    
    {
        Array2<char> array(4, 5);
        check_indexing(array, -1, -1);
        check_indexing(array, 0, -1);
        check_indexing(array, -1, 0);
        check_indexing(array, 4, 5);
        check_indexing(array, 3, 5);
        check_indexing(array, 4, 4);
    }

    // Check if specialized class gets called for bool
    // Check () operator on bool array
    // Check [] operator on bool array
    // Check [] outofrange indexing
    {
        Array2<bool> array(4, 5);
        assert(array.is_Bool());
        try {
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 5; j++) {
                    array(i, j) = ((j%2) == 0);
                }
            }
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 5; j++) {
                    assert(array[i][j] == ((j%2) == 0));
                }
            }
        } catch (...) {
            std::cout << "Checking () operator failed" << std::endl;
            return false;
        }
        
        check_indexing(array, -1, -1);
        check_indexing(array, 0, -1);
        check_indexing(array, -1, 0);
        check_indexing(array, 4, 5);
        check_indexing(array, 3, 5);
        check_indexing(array, 4, 4);
    }
    
    // Check for const ref for int
    {
        Array2<int> array(4,5);
        try {
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 5; j++) {
                    array[i][j] = i * j;
                }
            }
            
            const Array2<int> & const_array(array);

            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 5; j++) {
                    assert(const_array[i][j] == i * j);
                    assert(const_array(i, j) == i * j);
                }
            }
            // These lines should give compile time error.
#ifdef ASS2_CONST_CHECK
            const_array[2][3] = 3;
            const_array(3, 3) = 3;
#endif

        } catch (...) {
            std::cout << "Checking () operator failed" << std::endl;
            return false;
        }

    }
    
    // Check for const ref for bool
    {
        Array2<bool> array(4,5);
        try {
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 5; j++) {
                    array[i][j] = (j%2) == 0;
                }
            }
            
            const Array2<bool> & const_array(array);

            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 5; j++) {
                    assert(const_array[i][j] == ((j%2) == 0));
                    assert(const_array(i, j) == ((j%2) == 0));
                }
            }
            // These lines should give compile time error.
#ifdef ASS2_CONST_CHECK
            const_array[2][3] = true;
            const_array(3, 3) = true;
#endif

        } catch (...) {
            std::cout << "Checking () operator failed" << std::endl;
            return false;
        }

    }

    // Test queue for sequential operations
    {
        Queue<int> q;
        for(int i = 0; i < 50; i ++) {
            q.push_back(i);
        }
        for(int i = 0; i < 50; i ++) {
            assert(q.front() == i);
            q.pop_front();
        }

    }

    // Test queue for random data access
    {
        Queue<int> q;
        for(int i=0; i < 100000; i++) {
            q.push_back(1);
        }
        for(int i=0; i < 50000; i++) {
            q.pop_front();
        }
        for(int i=0; i < 100000; i++) {
            q.push_back(2);
        }
        
        // Test copy construction
        queue_test(q);
        
        for(int i=0; i < 50000; i++) {
            assert(q.front() == 1);
            q.pop_front();
        }
        for(int i=0; i < 100000; i++) {
            assert(q.front() == 2);
            q.pop_front();
        }
        
        try {
            q.pop_front();
        } catch(const char * ex) {
            assert(strcmp(ex, "Empty") == 0);
        }
        
        try {
            q.front();
        } catch(const char * ex) {
            assert(strcmp(ex, "Empty") == 0);
        }
    }

    // No requirement for assignment operator or default constructor
    {
        Queue<NoEqualOperator> q;
        
        for(int i=0; i < 100; i++) {
            q.push_back(NoEqualOperator(i));
        }
        for(int i=0; i < 100; i++) {
            assert((q.front()).value() == i);
            q.pop_front();
        }
    }
}



#endif

int
main()
{
#ifdef TEST
    test();
#endif

    return 0;
}
