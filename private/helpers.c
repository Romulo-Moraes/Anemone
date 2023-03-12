#include "helpers.h"
#include <stdlib.h>

unsigned short anemone__count_dashes_in_argument_name(char *argument){
  size_t argument_size = strlen(argument);
  unsigned short count = 0;

  while(argument[count] == '-'){
    count++;
  }

  return count;
}

anemone_optional_argument *anemone__create_optional_argument_node(char *long_name, char *short_name, anemone_bool require_value, anemone_bool required_argument){
  anemone_optional_argument *new_node = malloc(sizeof(anemone_optional_argument));

  if(new_node != NULL){
    new_node->long_name = long_name;
    new_node->short_name = short_name;
    new_node->require_value = require_value;
    new_node->required_argument = required_argument;
    new_node->found = FALSE;
    new_node->next = NULL;
  }

  return new_node;
}

void anemone__crash_the_program(unsigned short reason, char *bad_used_item){
  switch(reason){
  case BAD_LONG_ARGUMENT_NAME:
    printf("[ANEMONE PANIC] The setting 'long argument name' (%s) is not correct. Here's a correct example: --name [ANEMONE PANIC]\n", bad_used_item);
    break;

  case BAD_SHORT_ARGUMENT_NAME:
    printf("[ANEMONE PANIC] The setting 'short argument name' (%s) is not correct. Here's a correct example: -n [ANEMONE PANIC]\n", bad_used_item);
    break;

  case ARGUMENT_REQUIRED_WITH_UNKNOW_VALUE:
    printf("[ANEMONE PANIC] The setting 'argument required' (given value: %s) is not correct. Here're the possible ways: ANEMONE_TRUE and ANEMONE_FALSE macros (%d and %d respectively) [ANEMONE PANIC]\n", bad_used_item, ANEMONE_TRUE, ANEMONE_FALSE);
    break;

  case ARGUMENT_REQUIRES_A_VAUE_WITH_UNKNOW_VALUE:
    printf("%[ANEMONE PANIC] The setting 'argument requires a value' (given value: %s) is not correct. Here're the possible ways: ANEMONE_TRUE and ANEMONE_FALSE macros (%d and %d respectively) [ANEMONE PANIC]\n", bad_used_item, ANEMONE_TRUE, ANEMONE_FALSE);
    break;
  }

  exit(1);
}
