#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <strings.h>

#define DOG 0
#define FISH 1

struct Animal {
   int type;
   char * name;
   int number_of_legs;
   void (*play)(Animal *);
   void (*delet)(Animal *);
};

struct Dog {
   struct Animal base;
   int tag;
   void (*fetch)(Dog *);
};

struct Fish {
   struct Animal base;
   void (*swim)(Fish *);
};

void Dog_play(Animal *);
void Dog_fetch(Dog *);
Dog * Dog_new(int);
Dog * Animal_downcast_dog(Animal *);
void Dog_delet(Animal *);

void Fish_play(Animal *);
void Fish_swim(Fish *);
Fish * Fish_new();
Fish * Animal_downcast_fish(Animal *);
void Fish_delet(Animal *);
#endif
