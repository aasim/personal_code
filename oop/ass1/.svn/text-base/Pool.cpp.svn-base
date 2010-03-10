#include "Pool.hpp"
#include "Element.hpp"
#include "Node.hpp"


using namespace xml;

std::stack<void *, std::vector<void *> > Pool::small;
std::stack<void *, std::vector<void *> > Pool::large;

void * Pool::get_mem(unsigned int size, bool is_Element) {
     void * mem;
     if(!is_Element) {
          // Allocate from small
          if(Pool::small.empty()) {
               mem = malloc(size);
               return mem;
          } else {
               mem = Pool::small.top();
               Pool::small.pop();
               return mem;
          }

     } else { 
          // Allocate from large
          if(Pool::large.empty()) {
               mem = malloc(size);
               return mem;
          } else {
               mem = Pool::large.top();
               Pool::large.pop();
               return mem;
          }
     }
}

void Pool::free_mem(void * mem, bool is_Element) {
     if(is_Element) {
          Pool::large.push(mem);
     } else {
          Pool::small.push(mem);
     }
}

Pool::~Pool() {
     while(!Pool::small.empty()) {
          free(Pool::small.top());
          Pool::small.pop();
     }

     while(!Pool::large.empty()) {
          free(Pool::large.top());
          Pool::large.pop();
     }

}

