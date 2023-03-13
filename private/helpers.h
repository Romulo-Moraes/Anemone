#include "includes.h"
#include "library_crash_codes.h"
#ifndef ANEMONE_HEADER_DEFINED
#define ANEMONE_HEADER_DEFINED
#include "./../public/anemone_ap.h"
#endif

#define TRUE 1
#define FALSE 0





unsigned short count_dashes_in_argument_name(char *argument);
anemone_optional_argument *anemone__create_optional_argument_node(char *long_name, char *short_name, anemone_bool require_value, anemone_bool required_argument);
anemone_positional_argument *anemone__create_positional_argument_node(char *value);
void anemone__crash_the_program(unsigned short reason, char *bad_used_item);
void anemone__show_program_help();
void anemone__iterate_block_of_short_arguments(char *block_of_flags, anemone_optional_argument *list, int argc, char *argv[], unsigned short *current_iteration);
anemone_bool anemone__final_checks_in_compilation(anemone_struct lib);
