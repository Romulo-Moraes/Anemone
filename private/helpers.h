#ifndef ANEMONE_HELPERS_INCLUDED
#define ANEMONE_HELPERS_INCLUDED

#include "includes.h"

#define TRUE 1
#define FALSE 0


unsigned short count_dashes_in_argument_name(char *argument);
anemone_optional_argument *anemone__create_optional_argument_node(char *long_name, char *short_name, anemone_bool require_value, anemone_bool required_argument);
anemone_positional_argument *anemone__create_positional_argument_node(char *value);
void anemone__crash_the_program(unsigned short reason, char *bad_used_item, char *further_information, anemone_struct *lib);
void anemone__show_program_help(anemone_struct *lib);
void anemone__iterate_block_of_short_arguments(anemone_struct *lib, char *block_of_flags, anemone_optional_argument *list, int argc, char *argv[], unsigned short *current_iteration);
anemone_bool anemone__final_checks_in_compilation(anemone_struct lib);
void anemone__deallocate_all_if_allocation_failure(char **pointers_to_free,
                                                   size_t array_size);
void anemone__deallocate_all_to_exit(anemone_struct *lib);
void anemone__anemone_failure(int failure_id, anemone_struct *lib);

#endif
