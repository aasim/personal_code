#ifndef LIST_ANIMALPTR_HPP
#define LIST_ANIMALPTR_HPP

#include "Animals.hpp"

struct List_AnimalPtr;

struct List_AnimalPtr_Iterator {
     struct List_AnimalPtr * list;
     bool (*equal)(struct List_AnimalPtr_Iterator *, const struct List_AnimalPtr_Iterator &);
     void (* next)(struct List_AnimalPtr_Iterator *);
     Animal* (*deref)(struct List_AnimalPtr_Iterator *);
};

struct List_AnimalPtr {     
     Animal * animal;
     List_AnimalPtr * next;
     List_AnimalPtr * head;
     List_AnimalPtr * tail;
     void (* push_back)(List_AnimalPtr *, Animal *);
     List_AnimalPtr_Iterator (* end)(List_AnimalPtr *);
     List_AnimalPtr_Iterator (* begin)(List_AnimalPtr *);
     void (*dtor)(List_AnimalPtr *);
     
};


void List_AnimalPtr_ctor(List_AnimalPtr *);
void Animal_push_back(List_AnimalPtr *, Animal *);
List_AnimalPtr_Iterator List_Animal_end(List_AnimalPtr *);
List_AnimalPtr_Iterator List_Animal_begin(List_AnimalPtr *);
Animal* Animal_deref(List_AnimalPtr_Iterator *);
bool Animal_equal(List_AnimalPtr_Iterator *, const List_AnimalPtr_Iterator &);
void Animal_next(List_AnimalPtr_Iterator *);
void List_Animal_dtor(List_AnimalPtr *);

#endif
