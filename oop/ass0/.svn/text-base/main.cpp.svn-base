#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Animals.hpp"
#include "List_AnimalPtr.hpp"

int
main()
{
     Animal * a1 = (Animal *) Dog_new(35);

     // Dog name should be "Dog".
     printf("Dog name: %s\n", a1->name);
     printf("Number of legs: %d\n", a1->number_of_legs);

     a1->play(a1);

     Dog * d = Animal_downcast_dog(a1);
     printf("%d\n", d->tag);  // Derived data member.

     // Derived function member, should print exactly "Fetching...\n".
     // Must be a function pointer that is not in Animal.
     d->fetch(d);

     // Fish name should be "Fish", and should of course
     // have 0 legs.
     Animal *a2 = (Animal *) Fish_new();

     // Polymorphism, for Dog must call a different
     // function than for Fish.
     // For Dog, should print exactly "Woof!\n".
     // For Fish, should print exactly "Gurgle!\n".
     a2->play(a2);
     
     Fish *f = Animal_downcast_fish(a2);
     
     // Derived function member, should print exactly "Swimming...\n".
     // Must be a function pointer that is not in Animal.
     f->swim(f);
     
     // Uncommenting this line must give a compile time error.
     // Dog *d2 = Animal_downcast_fish(a2);
      
     // Downcast of wrong derived type must return null.
     assert(Animal_downcast_fish(a1) == 0);

     Dog *d2 = Dog_new(5678);

     //((Animal *) d2)->delet((Animal *) d2);

     List_AnimalPtr list;
     List_AnimalPtr_ctor(&list);
     list.push_back(&list, a1);
     list.push_back(&list, a2);

     for (List_AnimalPtr_Iterator it = list.begin(&list);
          !it.equal(&it, list.end(&list)); 
          it.next(&it)) {
             Animal *a = it.deref(&it);
             a->play(a);
     }

     // Must not delete the Animals.
     list.dtor(&list);
     
}
