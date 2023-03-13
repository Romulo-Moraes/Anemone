#include "anemone_ap.h"
#include "./../private/includes.h"

// Set default values to each field in anemone_struct
void initialize_lib(anemone_struct *lib){
  lib->necessary_positionals = 0;
  lib->compiled = ANEMONE_FALSE;
  lib->optional_argument_list = NULL;
  lib->positional_argument_list = NULL;
  lib->fetched_positionals = 0;
}


void create_positional_argument(anemone_struct *lib){
  lib->necessary_positionals += 1;
}



void create_optional_argument(anemone_struct *lib, char *long_name, char *short_name, anemone_require_value the_argument_requires_value, anemone_optional_required argument_required){
  char buffer_for_integer_conversion[16];
  
  anemone_optional_argument *new_node;

  
#include "./../private/helpers.h"
#include "./../private/list_operations.h"
#include "./../private/library_crash_codes.h"
  
  if(is_a_correct_long_name(long_name) == ANEMONE_TRUE){
    
    if(is_a_correct_short_name(short_name)){
      
      // Verifying whether this function argument is valid or not
      if(the_argument_requires_value == ANEMONE_TRUE || the_argument_requires_value == ANEMONE_FALSE){
	
	// Same for this
	if(argument_required == ANEMONE_TRUE || argument_required == ANEMONE_FALSE){
	  new_node = anemone__create_optional_argument_node(long_name, short_name, the_argument_requires_value, argument_required);
	  if(new_node != NULL){
	    anemone__insert_optional_argument_list(&(lib->optional_argument_list), new_node);
	  }else{
	    // TODO !!!!!
	    // Crash due fail on allocate dynamic memory
	  }
	}
	else{
	  // Crash due to unknow value
	  sprintf(buffer_for_integer_conversion, "%d", argument_required);
	  anemone__crash_the_program(ARGUMENT_REQUIRED_WITH_UNKNOW_VALUE, buffer_for_integer_conversion);
	}
      }
      else{
	// Crash due to unknow value
	sprintf(buffer_for_integer_conversion, "%d", the_argument_requires_value);
	anemone__crash_the_program(ARGUMENT_REQUIRES_A_VAUE_WITH_UNKNOW_VALUE, buffer_for_integer_conversion);
      }
    }
    else{
      // Crash due to incorrect long argument name format
      anemone__crash_the_program(BAD_SHORT_ARGUMENT_NAME, short_name);
    }
  }
  else{
    anemone__crash_the_program(BAD_LONG_ARGUMENT_NAME, long_name);
  }
}


anemone_bool compile(anemone_struct *lib, int argc, char *argv[], anemone_error_status *output){
#include "./../private/list_operations.h"
#include "./../private/helpers.h"
  
  unsigned short i = 1;
  anemone_optional_argument *search_result = NULL;
  size_t length_of_current_argument;
  anemone_positional_argument *new_positional_argument_node;
  
  while(i < argc){
    
    // Trying to find normal optional arguments
    if(strncmp(argv[i], "--", 2) == 0){
      if(is_a_correct_long_name(argv[i]) == ANEMONE_TRUE){
	search_result = anemone__search_value_in_optional_argument_list(lib->optional_argument_list, argv[i]);
	
	if(search_result != NULL){
	  search_result->found = ANEMONE_TRUE;
	  
	  if(search_result->require_value == ANEMONE_TRUE){
	    if(i != argc - 1){
	      search_result->optional_argument_value = argv[i + 1];
	      
	      i++;
	    }
	    else{
	      anemone__show_program_help();
	    }
	  }
	}
	else{
     	  anemone__show_program_help();
	}
      }
      else{
	// TODO !!!!!
      }

      // Trying to find short optional arguments
    }else if(strncmp(argv[i], "-", 1) == 0){
      if(is_a_correct_short_name(argv[i]) == ANEMONE_TRUE){
	anemone__iterate_block_of_short_arguments(argv[i], lib->optional_argument_list, argc, argv, &i);	
      }
    }else{
      new_positional_argument_node = anemone__create_positional_argument_node(argv[i]);

      if(new_positional_argument_node != NULL){
	anemone__insert_positional_argument_list(&(lib->positional_argument_list), new_positional_argument_node);
	lib->fetched_positionals += 1;
      }
      else{
	// TODO !!!!!
      }
      
    }

    i++;
  }

  if(anemone__final_checks_in_compilation(*lib) == ANEMONE_FALSE){
    anemone__show_program_help();
  }

  lib->compiled = ANEMONE_TRUE;
}

char *get_positional_argument(anemone_struct *lib, anemone_positional_argument_index index){
  
}

anemone_optional_return_value get_optional_argument(anemone_struct *lib, char *long_name, char *short_name){

}

anemone_bool is_a_correct_long_name(char *possible_long_name){
  size_t string_size = strlen(possible_long_name);
  anemone_bool is_correct_long_name = TRUE;

  // The long name should start with 2 dashes
  if(strncmp(possible_long_name, "--", 2) == 0){
    // Without the two dashes, the length of the name should be two or more
    if(string_size - 2 > 1){
      // The first digit after the dashes can't be a number, it's must be a letter of the alphabet
      if(!isdigit(possible_long_name[2]) && isalpha(possible_long_name[2])){
	// Loop to check if there's nothing beyond alphanumeric and new dashes characters
	for(unsigned int i = 3; i < string_size && is_correct_long_name == TRUE; i++){
	  if(!isalnum(possible_long_name[i]) && possible_long_name[i] != '-'){
	    // If something wrong appears, then set flag to FALSE
	    is_correct_long_name = FALSE;
	  }
	}

	return is_correct_long_name;
      }
    }
  }
  
  return FALSE;
}


anemone_bool is_a_correct_short_name(char *possible_short_name){
  size_t string_size = strlen(possible_short_name);
  anemone_bool correct_short_name = ANEMONE_TRUE;
  
  // The short argument name should start with only one dash in the begin
  if(strncmp(possible_short_name, "--", 2) != 0 && strncmp(possible_short_name, "-", 1) == 0){
    if(string_size - 1 >= 1){
      // This name must be of kind alphabet
      for(unsigned short i = 1; i < string_size && correct_short_name == ANEMONE_TRUE; i++){
	if(!isalpha(possible_short_name[1])){
	  correct_short_name = ANEMONE_FALSE;
	}
      }
    }
    else{
      correct_short_name = ANEMONE_FALSE;
    }
    
    

  }
  else{
    correct_short_name = ANEMONE_FALSE;
  }

  return correct_short_name;
}

anemone_bool is_a_correct_anemone_bool_value(int possible_anemone_bool_value){
  if(possible_anemone_bool_value == ANEMONE_TRUE || possible_anemone_bool_value == ANEMONE_FALSE){
    return ANEMONE_TRUE;
  }
  else{
    return ANEMONE_FALSE;
  }
}
