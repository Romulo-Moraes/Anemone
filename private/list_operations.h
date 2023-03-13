#ifndef ANEMONE_HEADER_DEFINED
#define ANEMONE_HEADER_DEFINED
#include "includes.h"
#include "./../public/anemone_ap.h"
#endif

void anemone__insert_optional_argument_list(anemone_optional_argument **list, anemone_optional_argument *new_node);
anemone_optional_argument *anemone__search_value_in_optional_argument_list(anemone_optional_argument *list, char *argument_name);
void anemone__insert_positional_argument_list(anemone_positional_argument **list, anemone_positional_argument *new_node);


 
