#include "anemone_ap.h"
#include <string.h>
#include <ctype.h>

// Set default values to each field in anemone_struct
void initialize_lib(anemone_struct *lib){
  lib->necessary_positionals = 0;
  lib->compiled = FALSE;
  lib->optional_argument_list = NULL;
  lib->positional_argument_list = NULL;
}


void create_positional_argument(anemone_struct *lib){
  lib->necessary_positionals += 1;
}


void create_optional_argument(anemone_struct *lib, char *long_name, char *short_name, anemone_require_value the_argument_requires_value, anemone_optional_required argument_required){

  /*
  extern unsigned short anemone__count_dashes_in_argument_name(char *argument);
  extern anemone_optional_argument *anemone__create_optional_argument_node(char *long_name, char *short_name, anemone_bool require_value, anemone_bool required_argument);
  extern void anemone__insert_optional_argument_list(anemone_optional_argument **list, anemone_optional_argument *new_node);
  extern void anemone__crash_the_program(unsigned short reason, char *bad_used_item);
  */

#include "./../private/helpers.h"
#include "./../private/list_operations.h"
#include "./../private/library_crash_codes.h"

  char buffer_for_integer_conversion[16];
  
  anemone_optional_argument *new_node = NULL;
  
  if(is_a_correct_long_name(long_name) == TRUE){
    
    if(is_a_correct_short_name(short_name)){
      
      // Verifying whether this function argument is valid or not
      if(the_argument_requires_value == TRUE || the_argument_requires_value == FALSE){
	
	// Same for this
	if(argument_required == TRUE || argument_required == FALSE){
	  new_node = anemone__create_optional_argument_node(long_name, short_name, the_argument_requires_value, argument_required);
	  if(new_node != NULL){
	    insert_optional_argument_list(&(lib->optional_argument_list), new_node);
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

  // The short argument name should start with only one dash in the begin
  if(strncmp(possible_short_name, "--", 2) != 0 && strncmp(possible_short_name, "-", 1) == 0){
    // Without the dash, the length of the flag name should be exactly one
    if(string_size - 1 == 1){
      // This name must be of kind alphabet
      if(isalpha(possible_short_name[1])){
	return TRUE;
      }
    }
  }

  return FALSE;
}

anemone_bool is_a_correct_anemone_bool_value(int possible_anemone_bool_value){
  if(possible_anemone_bool_value == ANEMONE_TRUE || possible_anemone_bool_value = ANEMONE_FALSE){
    return ANEMONE_TRUE;
  }
  else{
    return ANEMONE_FALSE;
  }
}
