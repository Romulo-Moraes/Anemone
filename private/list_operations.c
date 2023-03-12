#include "list_operations.h"


void anemone__insert_optional_argument_list(anemone_optional_argument **list, anemone_optional_argument *new_node){
  anemone_optional_argument *next, *current;
  
  if(*list == NULL){
    *list = new_node;
  }
  else{
    current = *list;
    next = current->next;

    while(next != NULL){
      current = next;
      next = next->next;
    }

    current->next = new_node;
  }
}
