#include <iostream>

#define MAX_DISTANCE 25

int sum_of_digits(int x) {
    int sum = 0;

    while(x) {
        sum += (x % 10);
        x /= 10;
    }

    return sum;
}

inline bool can_go(int x, int y) {
    return (sum_of_digits(x) + sum_of_digits(y)) <= MAX_DISTANCE;
}

int main() {

    int count = 0;

    // Check the top right quadrant including the pos x and y axes
    // and including the orignal point
    int x = 1000, y = 1000;
    int orig_y = 1000, orig_x = 1000;
    while(can_go(x, y)) {
        while(can_go(x, y)) {
#ifdef PRINT_COORDS
            std::cout << x << " " << y << " \n";
#endif
            y++;
            count++;
        }
        x++;
        y = orig_y;
    }

    // Check top left quadrant including the neg x axis
    // and excluding the orignal point
    x = orig_x - 1;
    y = orig_y;
    while(can_go(x, y)) {
        while(can_go(x, y)) {
#ifdef PRINT_COORDS
            std::cout << x << " " << y << " \n";
#endif
            y++;
            count++;
        }
        x--;
        y = orig_y;
    }


    // Check the bottom right quadrant including the neg y axis
    // and excluding the original point
    x = orig_x;
    y = orig_y - 1;
    while(can_go(x, y)) {
        while(can_go(x, y)) {
#ifdef PRINT_COORDS
            std::cout << x << " " << y << " \n";
#endif
            y--;
            count++;
        }
        x++;
        y = orig_y - 1;
    }

    // Checking the bottom left quadrant excluding the axes and the 
    // orignal point
    x = orig_x - 1;
    y = orig_y - 1;
    while(can_go(x, y)) {
        while(can_go(x, y)) {
#ifdef PRINT_COORDS
            std::cout << x << " " << y << " \n";
#endif
            y--;
            count++;
        }
        x--;
        y = orig_y - 1;
    }

    std::cout << count << "\n";

    return 0;
}

