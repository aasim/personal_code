#include <stdio.h>
#include <stdlib.h>

#include "Animals.hpp" 

void
Dog_play(Animal * a) {
     printf("Woof\n");
}

void 
Dog_fetch(Dog *d) {
     printf("Fetching...\n");
}

Dog *
Dog_new(int tag) {
     Dog * dog = (Dog *)malloc(sizeof(Dog));
     dog->base.name = strdup("Dog");
     dog->base.number_of_legs = 4;
     dog->base.play = &Dog_play;
     dog->base.type = DOG;
     dog->base.delet = &Dog_delet;
     dog->tag = tag;
     dog->fetch = &Dog_fetch;
     return dog;
}


void
Dog_delet(Animal * a) {
     Dog * d = (Dog *) a;
     free(d->base.name);
     free(d);
}
     

Dog *
Animal_downcast_dog(Animal * a) {
     if(a->type != DOG)
          return NULL;
     Dog * d = (Dog *) a;
     return d;
}


void
Fish_play(Animal * a) {
     printf("Gurgle\n");
}

void
Fish_swim(Fish * f) {
     printf("Swimming...\n");
}

Fish *
Fish_new() {
     Fish * fish = (Fish *)malloc(sizeof(Fish));
     fish->base.name = strdup("Fish");
     fish->base.number_of_legs = 0;
     fish->base.play = &Fish_play;
     fish->base.type = FISH;
     fish->base.delet = &Fish_delet;
     fish->swim = &Fish_swim;
     return fish;
}

Fish *
Animal_downcast_fish(Animal * a) {
     if(a->type != FISH)
          return NULL;
     Fish * f = (Fish *) a;
     return f;
}

void
Fish_delet(Animal * a) {
     Fish * f = (Fish *) a;
     free(f->base.name);
     free(f);
}
