#define ANEMONE_TRUE 1
#define ANEMONE_FALSE 0
#define ANEMONE_HEADER_DEFINED

typedef unsigned char anemone_require_value;
typedef unsigned short anemone_positional_argument_index;
typedef unsigned char anemone_bool;
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
    char *program_name;
    char *program_version;
    char *program_description;
    char *creators;
    char *special_thanks;
    char *email_contact;
} anemone_help_settings;

typedef struct _positional_description{
    char *positional_description;
    struct _positional_description *next;
} positional_arguments_description;

typedef struct{
    unsigned int necessary_positionals;
    unsigned int fetched_positionals;
    positional_arguments_description *list_of_descriptions;
    anemone_bool compiled;
    anemone_optional_argument *optional_argument_list;
    anemone_positional_argument *positional_argument_list;
    anemone_help_settings help_settings;
} anemone_struct;

void initialize_lib(anemone_struct *lib, char *program_name, char *program_version, char *program_depscription, char *creators, char *special_thanks, char *email_contact);
void create_positional_argument(anemone_struct *lib, char *argument_description);
void create_optional_argument(anemone_struct *lib, char *long_name, char *short_name, anemone_require_value the_argument_requires_value, anemone_optional_required argument_required);
void compile(anemone_struct *lib, int argc, char *argv[]);
char *get_positional_argument(anemone_struct lib, anemone_positional_argument_index index);
anemone_optional_return_value get_optional_argument(anemone_struct lib, char *argument_name);
unsigned int get_count_of_positional_arguments(anemone_struct lib);
anemone_bool is_valid_help_settings(char *program_name, char *program_version);
anemone_bool is_the_library_compiled(anemone_struct lib);
anemone_bool is_a_correct_long_name(char *possible_long_name);
anemone_bool is_a_correct_short_name(char *possible_short_name);
anemone_bool is_a_correct_anemone_bool_value(int possible_anemone_bool_value);
anemone_bool does_the_optional_argument_exist(anemone_struct lib, char *argument_name);
anemone_help_settings create_help_settings(char *program_name, char *program_version, char *program_description, char *creators, char *special_thanks, char *email_contact);
