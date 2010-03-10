#ifndef XMLP_POOL_H
#define XMLP_POOL_H

#include <stack>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <vector>


namespace xml {


     class Pool {
          public:
               static void * get_mem(unsigned int size, bool is_Element);
               static void free_mem(void * mem, bool is_Element);
               ~Pool();
          private:
               static std::stack <void *, std::vector<void *> > small;
               static std::stack <void *, std::vector<void *> > large;
     };
}
#endif
