#include "helpers.h"
#include "includes.h"


// Set default values to each field in anemone_struct
void initialize_lib(anemone_struct *lib, char *program_name, char *program_version, char *program_description, char *creators, char *special_thanks, char *email_contact){  
    anemone_help_settings help_settings;

    help_settings = create_help_settings(program_name, program_version, program_description, creators, special_thanks, email_contact);  

    lib->necessary_positionals = 0;
    lib->compiled = ANEMONE_FALSE;
    lib->optional_argument_list = NULL;
    lib->positional_argument_list = NULL;
    lib->fetched_positionals = 0;
    lib->help_settings = help_settings;
    lib->list_of_descriptions = NULL;
}

void create_positional_argument(anemone_struct *lib, char *argument_description)
{
    positional_arguments_description *new_node, *before, *current;

    new_node = (positional_arguments_description*) malloc(sizeof(positional_arguments_description));

    if(new_node != NULL){
	new_node->positional_description = argument_description;
	new_node->next = NULL;

	if(lib->list_of_descriptions != NULL){
	    current = lib->list_of_descriptions;

	    // TODO !!!!! Lists with head can save time to insert
	    // it's a suggestion to future updates
	    while(current != NULL){
		before = current;
		current = current->next;
	    }

	    before->next = new_node;
	}
	else{

	    lib->list_of_descriptions = new_node;
	}

	lib->necessary_positionals += 1;
    }
    else{
	anemone__anemone_failure(RESOURCES_ALLOCATION_FAILURE, lib);
    }
}

void create_optional_argument(anemone_struct *lib, char *long_name, char *short_name, anemone_require_value the_argument_requires_value, anemone_optional_required argument_required)
{
    char buffer_for_integer_conversion[16];

    anemone_optional_argument *new_node;

    if (is_a_correct_long_name(long_name) == ANEMONE_TRUE){

	if (is_a_correct_short_name(short_name)){

	    // Verifying whether this function argument is valid or not
	    if (the_argument_requires_value == ANEMONE_TRUE || the_argument_requires_value == ANEMONE_FALSE){

		// Same for this
		if (argument_required == ANEMONE_TRUE || argument_required == ANEMONE_FALSE){
		    new_node = anemone__create_optional_argument_node(long_name, short_name,
								      the_argument_requires_value,
								      argument_required);
                
		    if (new_node != NULL){
			anemone__insert_optional_argument_list(&(lib->optional_argument_list), new_node);
		    }
		    else{
			anemone__anemone_failure(RESOURCES_ALLOCATION_FAILURE, lib);
		    }
		}
		else{
		    // Crash due to unknow value
		    sprintf(buffer_for_integer_conversion, "%d", argument_required);
		    anemone__crash_the_program(ARGUMENT_REQUIRED_WITH_UNKNOW_VALUE, buffer_for_integer_conversion, "", lib);
		}
	    }
	    else{
		// Crash due to unknow value
		sprintf(buffer_for_integer_conversion, "%d", the_argument_requires_value);
		anemone__crash_the_program(ARGUMENT_REQUIRES_A_VALUE_WITH_UNKNOW_VALUE, buffer_for_integer_conversion, "", lib);
	    }
	}
	else{
	    // Crash due to incorrect long argument name format
	    anemone__crash_the_program(BAD_SHORT_ARGUMENT_NAME, short_name, "", lib);
	}
    }
    else{
	anemone__crash_the_program(BAD_LONG_ARGUMENT_NAME, long_name, "", lib);
    }
}

void anemone__collect_normal_optional_arguments(char *argv[], anemone_struct *lib, unsigned short *i, int argc) {
    anemone_optional_argument *search_result;

    
    if (is_a_correct_long_name(argv[*i]) == ANEMONE_TRUE){
	search_result = anemone__search_value_in_optional_argument_list(lib->optional_argument_list, argv[*i]);

	if (search_result != NULL){
	    search_result->found = ANEMONE_TRUE;

	    if (search_result->require_value == ANEMONE_TRUE){
		if (*i != argc - 1){
		    search_result->optional_argument_value = argv[*i + 1];
		    (*i)++;
		}
		else{
		    anemone__show_program_help(lib);
		}
	    }
	}
	else{
	    anemone__show_program_help(lib);
	}
    }
    else{
	anemone__show_program_help(lib);
    }
}

void anemone__collect_short_optional_arguments(char *argv[], unsigned short *i, anemone_struct *lib, int argc) {	
    if (is_a_correct_short_name(argv[*i]) == ANEMONE_TRUE){
	anemone__iterate_block_of_short_arguments(lib , argv[*i], lib->optional_argument_list, argc, argv, i);
    }  
}

void anemone__collect_positional_arguments(anemone_struct *lib, char *argv[], unsigned short i) {
    anemone_positional_argument *new_positional_argument_node;
    
    new_positional_argument_node = anemone__create_positional_argument_node(argv[i]);
	    
    if (new_positional_argument_node != NULL){
	anemone__insert_positional_argument_list(&(lib->positional_argument_list), new_positional_argument_node);
	lib->fetched_positionals += 1;
    }
    else{
	anemone__anemone_failure(RESOURCES_ALLOCATION_FAILURE, lib);
    }
}

anemone_bool compile(anemone_struct *lib, int argc, char *argv[])
{
    unsigned short i = 1;

    while (i < argc){

	// Trying to find normal optional arguments
	if (strncmp(argv[i], "--", 2) == 0){
	    anemone__collect_normal_optional_arguments(argv, lib, &i, argc);
	}
	else if (strncmp(argv[i], "-", 1) == 0){
	    // Trying to find short optional arguments
	    anemone__collect_short_optional_arguments(argv, &i, lib, argc);
	}
	else{
	    // Trying to find positional arguments
	    anemone__collect_positional_arguments(lib, argv, i);
	}

	i++;
    }

    if (anemone__final_checks_in_compilation(*lib) == ANEMONE_FALSE){
	anemone__show_program_help(lib);
    }

    lib->compiled = ANEMONE_TRUE;
}

void anemone__trigger_positional_out_of_range_error(anemone_positional_argument_index index, unsigned int limit, anemone_struct *lib) {
    char number_conversion_buffer[64];
    char number_conversion_buffer2[64];
    
    sprintf(number_conversion_buffer, "%d", index);
    sprintf(number_conversion_buffer2, "%d", limit);
    anemone__crash_the_program(GET_POSITIONAL_OUT_OF_RANGE, number_conversion_buffer, number_conversion_buffer2, lib);
}

char *get_positional_argument(anemone_struct lib, anemone_positional_argument_index index)
{
    anemone_positional_argument *return_value = lib.positional_argument_list;


    if (lib.compiled == ANEMONE_TRUE){
	if (index < lib.necessary_positionals){
	    for (unsigned int i = 0; i < index; i++){
		return_value = return_value->next;
	    }

	    return return_value->positional_argument_value;
	}
	else{
	    anemone__trigger_positional_out_of_range_error(index, lib.necessary_positionals, &lib);
	}
    }
    else{
	anemone__crash_the_program(TRIED_GET_POSITIONAL_VALUE_BEFORE_COMPILATION, "", "", &lib);
    }
}

unsigned int get_count_of_positional_arguments(anemone_struct lib){
    return lib.fetched_positionals;
}

anemone_optional_return_value get_optional_argument(anemone_struct lib, char *argument_name)
{
    anemone_optional_return_value return_value;
    anemone_optional_argument *search_result;

    search_result = anemone__search_value_in_optional_argument_list(lib.optional_argument_list, argument_name);

    if (search_result != NULL){
	if (search_result->found == ANEMONE_TRUE){
	    return_value.set = ANEMONE_TRUE;
	    return_value.value = search_result->optional_argument_value;
	}
	else{
	    return_value.set = ANEMONE_FALSE;
	    return_value.value = NULL;
	}
    }
    else{
	anemone__crash_the_program(GET_OPTIONAL_ARGUMENT_WITH_NONEXISTENT_FLAG, argument_name, "", &lib);
    }

    return return_value;
}

anemone_bool is_the_library_compiled(anemone_struct lib){
    return lib.compiled;
}

anemone_bool is_a_correct_long_name(char *possible_long_name)
{
    size_t string_size = strlen(possible_long_name);
    anemone_bool is_correct_long_name = TRUE;

    // The long name should start with 2 dashes
    if (strncmp(possible_long_name, "--", 2) == 0){
	// Without the two dashes, the length of the name should be two or more
	if (string_size - 2 > 1){
	    // The first digit after the dashes can't be a number, it's must be a letter of the alphabet
	    if (!isdigit(possible_long_name[2]) && isalpha(possible_long_name[2])){
		// Loop to check if there's nothing beyond alphanumeric and new dashes characters
		for (unsigned int i = 3; i < string_size && is_correct_long_name == TRUE; i++){
		    if (!isalnum(possible_long_name[i]) && possible_long_name[i] != '-'){
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

anemone_bool is_a_correct_short_name(char *possible_short_name)
{
    size_t string_size = strlen(possible_short_name);
    anemone_bool correct_short_name = ANEMONE_TRUE;

    // The short argument name should start with only one dash in the begin
    if (strncmp(possible_short_name, "--", 2) != 0 && strncmp(possible_short_name, "-", 1) == 0){
	if (string_size - 1 == 1){
	    if (!isalpha(possible_short_name[1])){
		correct_short_name = ANEMONE_FALSE;
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
    if (possible_anemone_bool_value == ANEMONE_TRUE || possible_anemone_bool_value == ANEMONE_FALSE){
	return ANEMONE_TRUE;
    }
    else{
	return ANEMONE_FALSE;
    }
}

anemone_bool is_valid_help_settings(char *program_name, char *program_version){
    if(program_name != NULL && strlen(program_name) >= 1 && program_version != NULL && strlen(program_version) >= 1){
	return ANEMONE_TRUE;
    }
    else{
	return ANEMONE_FALSE;
    }
}

anemone_help_settings create_help_settings(char *program_name, char *program_version, char *program_description, char *creators, char *special_thanks, char *email_contact)
{
    anemone_help_settings help_settings;

    if(is_valid_help_settings(program_name, program_version) == ANEMONE_TRUE){
	help_settings.program_name = program_name;
	help_settings.program_version = program_version;
    }
    else{
	anemone__crash_the_program(BAD_HELP_SETTING_VALUE, program_name, program_version, NULL);
    }

    // Description isn't necessary
    help_settings.program_description = program_description;
    // Creators aren't necessary
    help_settings.creators = creators;
    // Special thanks isn't necessary
    help_settings.special_thanks = special_thanks;
    // Email contact isn't necessary
    help_settings.email_contact = email_contact;

    return help_settings;
}
