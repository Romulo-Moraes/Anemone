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

void anemone__show_program_help(){
  puts("Program help here!!!");
  
  exit(1);
}


void anemone__iterate_block_of_short_arguments(char *block_of_flags, anemone_optional_argument *list, int argc, char *argv[], unsigned short *current_iteration){
  #include "list_operations.h"
  
  size_t flag_blocks_length = strlen(block_of_flags) - 1;
  anemone_optional_argument *search_result = NULL;
  char flag_format[3];

  for(unsigned short i = 1; i <= flag_blocks_length; i++){
    sprintf(flag_format, "-%c", block_of_flags[i]);

    search_result = anemone__search_value_in_optional_argument_list(list, flag_format);

    if(search_result != NULL){
      // The flag was found, set it to ANEMONE_TRUE
      search_result->found = ANEMONE_TRUE;
      // Checking if the argument requires a value
      if(search_result->require_value == ANEMONE_TRUE){
	// If requires a value and it's in the last position of the short arguments block, then...
	if(i == flag_blocks_length){
	  // If it needs a value and isn't in the last position in argv, then...
	  if(*current_iteration != argc - 1){
	    
	    search_result->optional_argument_value = argv[*current_iteration + 1];
	    
	    *current_iteration += 1;
	  }
	  // Else a crash is given due to no value passed to this flag
	  else{
	    anemone__show_program_help();
	  }
	}
	// If not in last position, then a crash is given, showing the help message
	else{
	  anemone__show_program_help();
	}
      }
    }else{
      anemone__show_program_help();
    }
  }
}



anemone_positional_argument *anemone__create_positional_argument_node(char *value){
  anemone_positional_argument *new_node = malloc(sizeof(anemone_positional_argument));

  if(new_node != NULL){
    new_node->positional_argument_value = value;
    new_node->next = NULL;
  }

  return new_node;
}


anemone_bool anemone__final_checks_in_compilation(anemone_struct lib){
  if(lib.fetched_positionals != lib.necessary_positionals){
    return ANEMONE_FALSE;
  }

  for(anemone_optional_argument *runner = lib.optional_argument_list; runner != NULL; runner = runner->next){
    if(runner->required_argument == ANEMONE_TRUE && runner->found == ANEMONE_FALSE){
      return ANEMONE_FALSE;
    }
  }

  return ANEMONE_TRUE;
}
