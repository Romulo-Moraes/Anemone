#define ANEMONE_TRUE 1
#define ANEMONE_FALSE 0
#define ANEMONE_HEADER_DEFINED

typedef unsigned char anemone_require_value;
typedef unsigned short anemone_positional_argument_index;
typedef unsigned char anemone_bool;
typedef unsigned char anemone_error_status;
typedef unsigned char anemone_optional_required;


typedef struct{
    anemone_bool set;
    char *value;
} anemone_optional_return_value;

typedef struct _anemone_positional_argument{
  char *positional_argument_value;
  struct _anemone_positional_argument *next;
} anemone_positional_argument;

typedef struct _anemone_optional_argument{
    char *long_name;
    char *short_name;
    char *optional_argument_value;
    anemone_bool require_value;
    anemone_bool required_argument;
    anemone_bool found;
    struct _anemone_optional_argument *next;
} anemone_optional_argument;

typedef struct{
    unsigned int necessary_positionals;
    unsigned int fetched_positionals;
    anemone_bool compiled;
    anemone_optional_argument *optional_argument_list;
    anemone_positional_argument *positional_argument_list;
} anemone_struct;

void initialize_lib(anemone_struct *lib);   
void create_positional_argument(anemone_struct *lib);
void create_optional_argument(anemone_struct *lib, char *long_name, char *short_name, anemone_require_value the_argument_requires_value, anemone_optional_required argument_required);
anemone_bool compile(anemone_struct *lib, int argc, char *argv[], anemone_error_status *output);                               
char *get_positional_argument(anemone_struct *lib, anemone_positional_argument_index index);                                
anemone_optional_return_value get_optional_argument(anemone_struct *lib, char *long_name, char *short_name);
anemone_bool is_a_correct_long_name(char *possible_long_name);
anemone_bool is_a_correct_short_name(char *possible_short_name);
anemone_bool is_a_correct_anemone_bool_value(int possible_anemone_bool_value);
