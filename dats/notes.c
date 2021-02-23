#include <stdio.h>

#include "notes.h"

void print_all_list_n_r(list_n_r *nr){
   for (list_n_r *p = nr; p!=NULL; p = p->next){
     switch(p->type){
	     case SYM_NOTE:  printf("NOTE length: %d frequency: %f\n", p->length, p->frequency); break;
     case SYM_REST: printf("REST length: %d\n", p->length); break;

     }


   }

}
