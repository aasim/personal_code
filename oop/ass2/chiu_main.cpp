#include "Array2.hpp"
#include "Queue.hpp"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <cstddef>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;
using namespace ass2;

// Number of outer iterations for the Array2 check.  A single Array2
// object is checked in each outer iteration.
const int ARRAY2_N_OUTER = 100;
// Number of inner iterations for the Array2 check.  This
// indicates how many operations are done on it.
const int ARRAY2_N_INNER = 100;

// Probability of a throw during object construction, for testing
// Queue class.
const double QUEUE_OBJ_THROW_PROBABILITY = 0.001;
// Number of queue operations to do.  The actual number is
// slightly less.
const size_t QUEUE_ITERATIONS = 1000000;
// Probability of switching from a growing queue to a shrinking
// queue.
const double QUEUE_GROW_SWITCH_PROBABILITY = .0001;
// Probability of adding an element when the queue is in the growing
// phase.
const double QUEUE_GROWING_ADD_PROBABILITY = 0.60;

// Number of bytes allocated on heap.  For testing that Array2<bool>
// specialization really is using less memory.
size_t bytesAllocated;

#ifndef CS540_VALGRIND_TEST
    void *operator new(size_t sz) {
        void *vp = malloc(sz + 8);
        *(size_t *) vp = sz;
        bytesAllocated += sz;
        return (char *) vp + 8;
    }
    void operator delete(void *vp) {
        vp = (char *) vp - 8;
        size_t sz = *(size_t *) vp;
        free(vp);
        bytesAllocated -= sz;
    }
#endif

// Convenience function, generates in range [low, high], inclusive.
size_t
rand(size_t low, size_t high) {
    size_t range = 1 + high - low;
    return low + drand48()*range;
}

/* -------------------------------------------------------------------------
 * Classes for Array2 tests.
 */

// This class allocates some memory so it can be used
// to test for object/memory leaks due to not calling
// the destructor when needed.
//
// It acts like an int, mostly.
class Integer {
        friend bool operator==(const Integer &, const Integer &);
    public:
        Integer() : ip(new int) {}
        Integer(int sz) : ip(new int) { *ip = sz; }
        Integer(const Integer &i) : ip(new int) { *ip = *i.ip; }
        ~Integer() { delete ip; }
        Integer &operator=(const Integer &i) {
            *ip = *i.ip;
            return *this;
        }
    private:
        int *const ip;
};

inline bool
operator==(const Integer &i1, const Integer &i2) {
    return *i1.ip == *i2.ip;
}

/*
 * Traits class.
 */

// There is no point for a generic definition of this.
template <typename T> class Traits;

template <>
class Traits<Integer> {
    public:
        static Integer generate_random_value() {
            return Integer(rand(0, 1000000));
        }
};

template <>
class Traits<bool> {
    public:
        static bool generate_random_value() {
            return rand(0, 1);
        }
};

/*
 * Template to check Array2.  A template is used so that
 * we can reuse the code to check for bool.
 */

template <typename T>
void
check_Array2() {

    for (int i = 0; i < ARRAY2_N_OUTER; i++) {

        size_t rows = rand(1, 10);
        size_t cols = rand(1, 10);
        Array2<T> a(rows, cols);
        // For verifying that the array is working.
        T *verify = new T[rows * cols];

        /*
         * Initialize arrays to something.
         */
        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
                a(r, c) = *(verify + r*cols + c)
                 = Traits<T>::generate_random_value();
            }
        }

        /*
         * Each iteration of this loops does one randomly
         * selected operation.
         */

       for (int j = 0; j < ARRAY2_N_INNER; j++) {

            // Randomly pick range-checking or not.
            // True branch is range-checking.
            if (rand(0, 1) == 0) {

                /*
                 * Test a range-checking operation.
                 */

                // Generate possibly out of bounds indices.
                size_t r = rand(0, rows + 3);
                size_t c = rand(0, cols + 3);

                try {
                    // Randomly pick read or write operation.
                    // True branch is write op.
                    if (rand(0, 1) == 0) {
                        T t = Traits<T>::generate_random_value();
                        a[r][c] = t;
                        // If we reach here, must be in range.
                        assert(r < rows && c < cols);
                        // Write to verify array.
                        *(verify + r*cols + c) = t;
                    // Do read.
                    } else {
                        T t = a[r][c];
                        // If we reach here, must be in range.
                        assert(r < rows && c < cols);
                        // Check with verify array.
                        assert(t == *(verify + r*cols + c));
                    }
                } catch (const OutOfRange &) {
                    // If we reach here, must be out of range.
                    assert(r >= rows || c >= cols);
                }

            // Test a non range-checking op.
            } else {

                // Pick in range indices.
                size_t r = rand(0, rows - 1);
                size_t c = rand(0, cols - 1);

                try {
                    // Randomly pick write or read op.
                    // True branch is write op.
                    if (rand(0, 1) == 0) {
                        // Check set.
                        a(r, c) = *(verify + r*cols + c)
                         = Traits<T>::generate_random_value();
                    } else {
                        // Check read, also verify values.
                        assert(a(r, c) == *(verify + r*cols + c));
                    }
                } catch (...) {
                    // No exceptions should be thrown.
                    assert(false);
                }
            }
        }

        delete [] verify;
    }
}

/* -------------------------------------------------------------------------
 * Classes for Queue tests.
 */

/*
 * The objects that we put in the Queue must be able to randomly
 * throw exceptions to test for exception safety.
 *
 * The classes below are to provide that.
 */

// The exception to throw.
struct QueueObjException {};

/*
class QueueObj;
inline bool operator==(const QueueObj &qo1, const QueueObj &qo2) {
    return *qo1.ip == *qo2.ip;
}
*/

class QueueObj {
        // friend bool operator==(const QueueObj<T1> &, const QueueObj<T2> &);
    public:
        QueueObj(int i_) : ip(new int) { *ip = i_; }
        QueueObj(const QueueObj &qo);
        ~QueueObj() { delete ip; }
    private:
        // Disallow default constructor.
        QueueObj();
        // Disallow assignment.
        QueueObj &operator=(const QueueObj &);
    public:
        operator int() const { return *ip; }
    private:
        int *const ip;
};

QueueObj::QueueObj(const QueueObj &qo) : ip(new int) {
    // Randomly decide to throw.
    double x = drand48();
    if (x < QUEUE_OBJ_THROW_PROBABILITY) {
        delete ip;
        throw QueueObjException();
    }
    *ip = *qo.ip;
}

/* -------------------------------------------------------------------------
 * Classes for Array2 tests.
 */

int
main(int argc, char *argv[]) {

    // Verify that bool specialization uses
    // less memory.
    {
        size_t base = bytesAllocated;
        int char_usage, bool_usage;
        {
            Array2<char> a(10, 100);
            char_usage = bytesAllocated - base;
        }
        {
            Array2<bool> a(10, 100);
            bool_usage = bytesAllocated - base;
        }
        // If both of these are zero, then we are running on valgrind,
        // and the check will not work.
        if (char_usage == 0) {
            assert(bool_usage == 0);
        } else {
            assert(bool_usage < char_usage);
        }
        printf("For an Array2 containing 1,000 elements, "
         "char used %d bytes and bool used %d bytes.\n",
         char_usage, bool_usage);
    }

    // Check Array2 correctness.
    check_Array2<Integer>();
    check_Array2<bool>();

    // Check Array2 const behavior.
    {
        Array2<int> a1(2, 2);
        Array2<bool> a2(2, 2);

        a1[0][0] = 0;
        a1[0][1] = 1;
        a1[1][0] = 2;
        a1[1][1] = 3;

        a2[0][0] = false;
        a2[0][1] = true;
        a2[1][0] = false;
        a2[1][1] = true;

        const Array2<int> &cr1(a1);
        int i;

        // Can read from const.
        i = cr1(1, 1);
        i = cr1[1][1];

        const Array2<bool> &cr2(a2);
        bool b;
        b = cr2(1, 1);
        b = cr2[1][1];

        // There should be an error on each line below.
        #ifdef CS540_CONST_TEST
            cr1(1, 1) = 1;
            cr1[1][1] = 1;
            cr2(1, 1) = true;
            cr2[1][1] = true;
        #endif
    }

    /* -----------------------------------------------------------------------
     * Test Queue class.
     *
     * This test randomly adds/removes elements.  It tries to make
     * sure that the queue is grown to sufficiently large to trigger
     * expansion, but also make sure that it goes to empty sometimes too.
     */
    {
    Queue<QueueObj> q;
    queue<int> verify;  // Use std::queue to verify that Queue is correct.

    // Keep some statistics.
    double avg_queue_size = 0;
    size_t max_queue_size = 0;
    double add_prob = QUEUE_GROWING_ADD_PROBABILITY;
    int switch_count = 0;
    int exception_count = 0;

    // Flag used to keep track of whether we ar 
    bool growing = true;

    for (size_t i = 0; i < QUEUE_ITERATIONS; i++) {

        // Occasionally switch from growing to shrinking.
        if (growing) {
            if (drand48() < QUEUE_GROW_SWITCH_PROBABILITY) {
                growing = false;
                add_prob = 1 - QUEUE_GROWING_ADD_PROBABILITY;
                switch_count++;
            }
        }

        // Give the percentage of push slightly more, so that
        // length will increase.
        double r = drand48();
        if (r < add_prob) {
            try {
                // If the first throws, the verify push is not
                // executed, leaving the state unchanged.
                q.push_back(i);
                verify.push(i);
            } catch (const QueueObjException &) {
                // No need to do anything.
                //printf("Queue exception: %d.\n", int(i));
                exception_count++;
            } catch (...) {
                assert(false);
            }
        } else {
            if (verify.size() > 0) {
                q.pop_front();
                verify.pop();
            } else {
                // Empty, so switch to growing.
                growing = true;
                add_prob = QUEUE_GROWING_ADD_PROBABILITY;
            }
        }

        // Verify that contents are correct.
        if (verify.size() > 0) {
            assert(verify.front() == q.front());
        }

        avg_queue_size += verify.size()/double(QUEUE_ITERATIONS);
        max_queue_size = max(max_queue_size, verify.size());

        //printf("size: %d\n", int(verify.size()));
    }
    

    printf("avg size: %f, max size: %d\n", avg_queue_size, int(max_queue_size));
    printf("no. of switches: %d, no. of exceptions: %d\n",
     switch_count, exception_count);
    } 
}



/* vim: set ai sts=4 sw=4 et: */
