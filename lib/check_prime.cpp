#include "check_prime.hpp"

namespace aasim_lib {
    // Using trial division method
    // Works only for positive numbers

    bool 
    check_prime_basic(double number) {

        if(number < 0) throw "Bad input";

        for(int i = 2; i <= sqrt(number); i++) {
            double tmp = number / (double) i;
            if(tmp  == (int) tmp) return false;
        }

        return true;
    }
}
