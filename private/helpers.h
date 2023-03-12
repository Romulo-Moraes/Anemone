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

void anemone__crash_the_program(unsigned short reason, char *bad_used_item);
