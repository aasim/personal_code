// http://www.careercup.com/question?id=310994
// Implementation of solution by Prax

#include <iostream>

#define N       20
#define ODD     1
#define EVEN    0

int main() {

    int a1[N] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 }; 
    int a2[N] = { 21,22,23,24,25,27,26,28,29,30,31,32,33,34,35,36,37,38,39,40};

    int rep = 0;
    int swap;

    // a1 should contain odd numbers, a2 should contain even numbers
    for (int i = 0, j = 0 ; rep < N/2 ;) {
        if ((a1[i]%2 == ODD) && (a2[j]%2 == EVEN)) {
            rep++;
            swap = a1[i];
            a1[i] = a2[j];
            a2[j] = swap;

            i++; j++;

        } else {
            if (a1[i]%2 == EVEN)
                i++;

            if (a2[j]%2 == ODD)
                j++;
        }
    }


    for(int i=0; i < N; i++)
        std::cout << a1[i] << " " << a2[i] << std::endl;
}
