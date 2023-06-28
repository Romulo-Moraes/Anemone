#include "includes.h"

anemone_optional_argument *anemone__search_value_in_optional_argument_list(anemone_optional_argument *list, char *argument_name){
  anemone_optional_argument *runner = list;

  while(runner != NULL){
    if(strcmp(runner->long_name, argument_name) == 0 || strcmp(runner->short_name, argument_name) == 0){
      return runner;
    }

    runner = runner->next;
  }
  
  return runner;
}

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


void anemone__insert_positional_argument_list(anemone_positional_argument **list, anemone_positional_argument *new_node){
  anemone_positional_argument *current, *next;
  
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

