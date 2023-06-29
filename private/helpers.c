#include "./includes.h"
#include <stdio.h>

anemone_positional_argument *anemone__create_positional_argument_node(char *value){
    anemone_positional_argument *new_node = malloc(sizeof(anemone_positional_argument));

    if(new_node != NULL){
	new_node->positional_argument_value = value;
	new_node->next = NULL;
    }

    return new_node;
}

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

void anemone__crash_the_program(unsigned short reason, char *bad_used_item, char *further_information, anemone_struct *lib){
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

    case ARGUMENT_REQUIRES_A_VALUE_WITH_UNKNOW_VALUE:
	printf("[ANEMONE PANIC] The setting 'argument requires a value' (given value: %s) is not correct. Here're the possible ways: ANEMONE_TRUE and ANEMONE_FALSE macros (%d and %d respectively) [ANEMONE PANIC]\n", bad_used_item, ANEMONE_TRUE, ANEMONE_FALSE);
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

    if(lib != NULL)
	anemone__deallocate_all_to_exit(lib);
    
    exit(1);
}

void anemone__print_positional_arguments_and_descriptions(anemone_struct *lib) {
    size_t positional_argument_index = 1;
    
    for(positional_arguments_description *runner = lib->list_of_descriptions;
	runner != NULL; runner = runner->next){
	
	printf("Positional argument %" PRIu64 ": %s\n",
	       positional_argument_index, runner->positional_description);
    
	positional_argument_index++;
    }

    puts("");
}

void anemone__print_optional_arguments_and_descriptions(anemone_struct *lib) {
    anemone_bool first_loop = TRUE;
    
    printf("Optional arguments: ");
    
    for(anemone_optional_argument *runner = lib->optional_argument_list;
	runner != NULL; runner = runner->next){

	if(first_loop == FALSE){
	    printf("                    ");
	}else{
	    first_loop = FALSE;
	}
	
	printf("%s / %s %s%s\n", runner->short_name,
	       runner->long_name, runner->require_value == ANEMONE_TRUE ? "<value>" : "",
	       runner->required_argument == ANEMONE_TRUE ? " (REQUIRED)" : "");
    }

    puts("");
}

void anemone__print_program_help_header(anemone_struct *lib) {
    printf("Program: %s, version: %s\n", lib->help_settings.program_name, lib->help_settings.program_version);

    if(lib->help_settings.program_description != NULL){
	printf("\n%s\n\n", lib->help_settings.program_description == NULL ? "" : lib->help_settings.program_description);
    }else{
	puts("");
    }
}


void anemone__print_command_separated_values(char *token_pointer) {
    anemone_bool is_the_first_loop = TRUE;
    
    do{
	if(is_the_first_loop == ANEMONE_FALSE){
	    printf(", ");
	}

	printf("%s", token_pointer);

	is_the_first_loop = FALSE;
    }while((token_pointer = strtok(NULL, ";")) != NULL);
}

void anemone__print_program_creators(anemone_struct *lib) {
    char *creators_pointer, *token_pointer;
    size_t creators_string_length;
    anemone_bool is_the_first_loop = ANEMONE_TRUE;
    
    if(lib->help_settings.creators != NULL && (creators_string_length = strlen(lib->help_settings.creators)) >= 1){
	creators_pointer = (char*) malloc(sizeof(char) * creators_string_length + 1);
	
	if(creators_pointer != NULL){
	    strcpy(creators_pointer, lib->help_settings.creators);

	    token_pointer = strtok(creators_pointer, ";");
	    printf("Creators: ");

	    anemone__print_command_separated_values(token_pointer);

	    free(creators_pointer);
	}
	else{
	    printf("Creators: %s", lib->help_settings.creators);
	}

	puts("");
    }
}


void anemone__print_special_thanks(anemone_struct *lib) {
    anemone_bool is_the_first_loop = TRUE;
    size_t special_thanks_string_length;
    char *special_thanks_pointer, *token_pointer;
    
    if(lib->help_settings.special_thanks != NULL && (special_thanks_string_length = strlen(lib->help_settings.special_thanks)) >= 1){

	special_thanks_pointer = (char*) malloc(special_thanks_string_length + 1);

	if(special_thanks_pointer != NULL){
	    strcpy(special_thanks_pointer, lib->help_settings.special_thanks);

	    token_pointer = strtok(special_thanks_pointer, ";");

	    printf("Special thanks to: ");

	    anemone__print_command_separated_values(token_pointer);

	    free(special_thanks_pointer);
	}
	else{
	    printf("Special thanks to: %s\n", lib->help_settings.special_thanks);
	}

	puts("");
    }
}

void anemone__print_email_contact(anemone_struct *lib) {   
    if(lib->help_settings.email_contact != NULL && strlen(lib->help_settings.email_contact) >= 1){
	printf("E-mail contact: %s\n", lib->help_settings.email_contact);
    }
}

void anemone__show_program_help(anemone_struct *lib){
    anemone__print_program_help_header(lib);
  
    anemone__print_positional_arguments_and_descriptions(lib);

    anemone__print_optional_arguments_and_descriptions(lib);

    anemone__print_program_creators(lib);

    anemone__print_special_thanks(lib);

    anemone__print_email_contact(lib);

    anemone__deallocate_all_to_exit(lib);
    
    exit(0);
}

void anemone__deallocate_list_of_descriptions(
					      positional_arguments_description *runner) {

    positional_arguments_description *x;
    
    while (runner != NULL) {
	x = runner;
	runner = runner->next;
	
	free(x);
    }
}

void anemone__deallocate_list_of_optional_arguments(
						    anemone_optional_argument *runner) {

    anemone_optional_argument *x;

    while(runner != NULL){
	x = runner;
	runner = runner->next;

	free(x);
    }
}

void anemone__deallocate_list_of_positional_arguments_value(
							    anemone_positional_argument *runner) {

    anemone_positional_argument *x;

    while(runner != NULL){
	x = runner;
	runner = runner->next;

	free(x);
    }
}

void anemone__deallocate_all_to_exit(anemone_struct *lib) {
    anemone__deallocate_list_of_descriptions(lib->list_of_descriptions);

    anemone__deallocate_list_of_optional_arguments(lib->optional_argument_list);

    anemone__deallocate_list_of_positional_arguments_value(lib->positional_argument_list);
}

void anemone__iterate_block_of_short_arguments(anemone_struct *lib, char *block_of_flags, anemone_optional_argument *list, int argc, char *argv[], unsigned short *current_iteration){  
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
		    else{
			// Else a crash is given due to no value passed to this flag
			anemone__show_program_help(lib);
		    }
		}
		else{
		    // If not in last position, then a crash is given, showing the help message
		    anemone__show_program_help(lib);
		}
	    }
	}else{
	    anemone__show_program_help(lib);
	}
    }
}


anemone_bool anemone__final_checks_in_compilation(anemone_struct lib){
    /*
      Checking if the total of fetched positionals arguments is
      equal to the necessary, since a defined positional is always required,
      any mismatch is not acceptable
    */
    if(lib.fetched_positionals != lib.necessary_positionals){
	return ANEMONE_FALSE;
    }


    /*
      Checking if all defined required arguments were found,
      if not, return error.
    */
    for(anemone_optional_argument *runner = lib.optional_argument_list; runner != NULL; runner = runner->next){
	if(runner->required_argument == ANEMONE_TRUE && runner->found == ANEMONE_FALSE){
	    return ANEMONE_FALSE;
	}
    }

    /* No problems found, everything ok */
    return ANEMONE_TRUE;
}

void anemone__deallocate_all_if_allocation_failure(char **pointers_to_free, size_t array_size){
    for(size_t i = 0; i < array_size; i++){
	if(pointers_to_free[i] != NULL){
	    free(pointers_to_free[i]);
	}
    }
}

void anemone__anemone_failure(int failure_id, anemone_struct *lib) {
    switch(failure_id){
    case RESOURCES_ALLOCATION_FAILURE:
	puts("[ANEMONE FAILURE] The anemone library failed to allocate memory [ANEMONE FAILURE]");
	break;
    }

    anemone__deallocate_all_to_exit(lib);

    exit(1);
}
