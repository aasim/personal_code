// http://www.careercup.com/question?id=311074

#include "../lib/check_prime.hpp"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>

int main() {
    using namespace aasim_lib;

    // Generate random 9 digit number
    // rand() generates a prime number between 0 to RAND_MAX
    // RAND_MAX is defined to be 2147483647 in the GNU C library
    
    srand(time(NULL));

    double number = rand() % 899999999 + 100000000;

    std::cout 
        << std::fixed
        << std::setprecision(0)
        << number 
        << " is prime? : " 
        << std::boolalpha 
        << check_prime_basic(number)
        << std::endl;

    int first_3_digits = number / 1000000;

    std::cout 
        << std::fixed 
        << first_3_digits 
        << " is prime? : " 
        << std::boolalpha 
        << check_prime_basic(first_3_digits)
        << std::endl;

    int middle_2_digits = (int)number % 1000000;
    middle_2_digits /= 10000;

    std::cout 
        << std::fixed 
        << middle_2_digits 
        << " is prime? : " 
        << std::boolalpha 
        << check_prime_basic(middle_2_digits)
        << std::endl;

    int last_4_digits = (int)number % 10000;
    
    std::cout 
        << std::fixed 
        << last_4_digits 
        << " is prime? : " 
        << std::boolalpha 
        << check_prime_basic(last_4_digits)
        << std::endl;

    return 0;
}
