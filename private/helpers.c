#include "helpers.h"
#include <stdlib.h>
#include <inttypes.h>
#include "./includes.h"

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

void anemone__crash_the_program(unsigned short reason, char *bad_used_item, char *further_information){
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

  case TRIED_GET_POSITIONAL_VALUE_BEFORE_COMPILATION:
    printf("[ANEMONE PANIC] The library tried to get a positional argument before compile the arguments! [ANEMONE PANIC]\n");
    break;

  case GET_POSITIONAL_OUT_OF_RANGE:
    printf("[ANEMONE PANIC] The 'get positional argument' feature tried to fetch an argument in the position %s but the max range was %s! [ANEMONE PANIC]\n", bad_used_item, further_information);
    break;

  case GET_OPTIONAL_ARGUMENT_WITH_NONEXISTENT_FLAG:
    printf("[ANEMONE PANIC] The feature 'get optional argument' was used with an unknow flag (%s)! [ANEMONE PANIC]\n", bad_used_item);
    break;

  case BAD_HELP_SETTING_VALUE:
    printf("[ANEMONE PANIC] Bad arguments when creating help settings, program name and program version must be different of NULL and length >= 1. [ANENOME PANIC]\n");
    break;
  }

  exit(1);
}

char *anemone__realloc_creators_and_special_thanks_pointer_if_necessary(char *the_pointer, size_t creators_string_length, size_t special_thanks_string_length){
  if(special_thanks_string_length < creators_string_length){
    return the_pointer;
  }
  else{
    free(the_pointer);

    the_pointer = (char*) malloc(sizeof(char) * special_thanks_string_length + 1);

    return the_pointer;
  }
}

void anemone__show_program_help(anemone_struct *lib){
  char *creators_and_special_thanks_pointer;
  char *token_pointer;
  size_t positional_argument_index = 1;
  anemone_bool is_the_first_loop = TRUE;
  size_t creators_string_length;
  size_t special_thanks_string_length;

  printf("Program: %s, version: %s\n", lib->help_settings.program_name, lib->help_settings.program_version);

  printf("\n%s\n\n", lib->help_settings.program_description == NULL ? "" : lib->help_settings.program_description);
  
  // Print all positional arguments and their descriptions
  for(positional_arguments_description *runner = lib->list_of_descriptions; runner != NULL; runner = runner->next){
    printf("Positional argument %" PRIu32 ": %s\n", positional_argument_index, runner->positional_description);
    
    positional_argument_index++;
  }

  puts("");

  // Print all optional arguments and their features
  for(anemone_optional_argument *runner = lib->optional_argument_list; runner != NULL; runner = runner->next){
    printf("%s / %s %s%s\n", runner->short_name, runner->long_name, runner->require_value == ANEMONE_TRUE ? "<value>" : "", runner->required_argument == ANEMONE_TRUE ? " (REQUIRED)" : "");
  }

  puts("");

  // Print all creators if exist
  if(lib->help_settings.creators != NULL && (creators_string_length = strlen(lib->help_settings.creators)) >= 1){
    creators_and_special_thanks_pointer = (char*) malloc(sizeof(char) * creators_string_length + 1);

    if(creators_and_special_thanks_pointer != NULL){
      strcpy(creators_and_special_thanks_pointer, lib->help_settings.creators);

      token_pointer = strtok(creators_and_special_thanks_pointer, ";");
      printf("Creators: ");

      do{
        if(is_the_first_loop == ANEMONE_FALSE){
          printf(", ");
        }

        printf("%s", token_pointer);

        is_the_first_loop = FALSE;

      }while((token_pointer = strtok(NULL, ";")) != NULL);
    }
    else{
      printf("Creators: %s", lib->help_settings.creators);
    }
  }

  puts("");

  if(lib->help_settings.special_thanks != NULL && (special_thanks_string_length = strlen(lib->help_settings.special_thanks)) >= 1){
    is_the_first_loop = TRUE;

    creators_and_special_thanks_pointer = anemone__realloc_creators_and_special_thanks_pointer_if_necessary(creators_and_special_thanks_pointer, creators_string_length, special_thanks_string_length);

    if(creators_and_special_thanks_pointer != NULL){
      strcpy(creators_and_special_thanks_pointer, lib->help_settings.special_thanks);

      token_pointer = strtok(creators_and_special_thanks_pointer, ";");


      printf("Special thanks to: ");

      do{
        if(is_the_first_loop == ANEMONE_FALSE){
          printf(", ");
        }

        printf("%s", token_pointer);

        is_the_first_loop = FALSE;
      }while((token_pointer = strtok(NULL, ";")) != NULL);

      free(creators_and_special_thanks_pointer);
    }
    else{
      printf("Special thanks to: %s\n", lib->help_settings.special_thanks);
    }
  }

  puts("");

  if(lib->help_settings.email_contact != NULL && strlen(lib->help_settings.email_contact) >= 1){
    printf("E-mail contact: %s\n", lib->help_settings.email_contact);
  }

  // TODO !!!! All allocated resources must be deallocated
  exit(0);
}


void anemone__iterate_block_of_short_arguments(anemone_struct *lib, char *block_of_flags, anemone_optional_argument *list, int argc, char *argv[], unsigned short *current_iteration){
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
	    anemone__show_program_help(lib);
	  }
	}
	// If not in last position, then a crash is given, showing the help message
	else{
	  anemone__show_program_help(lib);
	}
      }
    }else{
      anemone__show_program_help(lib);
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

void anemone__deallocate_all_if_allocation_failure(char **pointers_to_free, size_t array_size){
    for(size_t i = 0; i < array_size; i++){
      if(pointers_to_free[i] != NULL){
        free(pointers_to_free[i]);
      }
    }
}