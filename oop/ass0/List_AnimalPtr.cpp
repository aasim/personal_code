#include <assert.h>
#include <stdlib.h>

#include "List_AnimalPtr.hpp"

void
List_AnimalPtr_ctor(List_AnimalPtr * list) {
     list->animal = NULL;
     
     List_AnimalPtr * end = (List_AnimalPtr *)malloc(sizeof(List_AnimalPtr));
     end->next = NULL;
     list->next = end;
     
     list->head = list->tail = list;
     list->push_back = &Animal_push_back;
     list->end = &List_Animal_end;
     list->begin = &List_Animal_begin;
     list->dtor = &List_Animal_dtor;
}

void 
List_Animal_dtor(List_AnimalPtr * list) {
     list = list->next;
     while(list->next != NULL) {
          List_AnimalPtr * temp = list;
          list = list->next;
          free(temp);
     }
     
     // Free the sentinel value
     free(list);
     
}


List_AnimalPtr_Iterator
List_Animal_end(List_AnimalPtr * list) {
     List_AnimalPtr_Iterator iter;
     iter.list = list->tail->next;
     return iter;
}

List_AnimalPtr_Iterator
List_Animal_begin(List_AnimalPtr * list) {
     List_AnimalPtr_Iterator iterator;
     iterator.list = list->head;
     iterator.equal = Animal_equal;
     iterator.next = Animal_next;
     iterator.deref = Animal_deref;
     
     return iterator;
}

Animal*
Animal_deref(List_AnimalPtr_Iterator *iter) {
     return iter->list->animal;
}

bool 
Animal_equal(List_AnimalPtr_Iterator * iter_s, const List_AnimalPtr_Iterator &iter) {
     if(iter_s->list == iter.list) 
          return true;
     else
          return false;
}

void 
Animal_next(List_AnimalPtr_Iterator * iter) {
     iter->list = iter->list->next;
}

void
Animal_push_back(List_AnimalPtr * list, Animal * a) {
     assert(list != NULL);
     assert(a != NULL);

     
     if(list->head->animal == NULL) {
          list->head->animal = a;
          
          return;
     }

     List_AnimalPtr * node = (List_AnimalPtr *)malloc(sizeof(List_AnimalPtr));

     node->animal = a;
     node->next = list->tail->next;

     list->tail->next = node;
     list->tail = node;
}

