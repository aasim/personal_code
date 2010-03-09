#include "../lib/check_prime.hpp"

#include <iostream>

int main() {
    int i;
    using namespace aasim_lib;

    while(true) {
        std::cin >> i;

        std::cout << std::boolalpha << check_prime_basic(i) << std::endl;

    }

    return 0;
}
