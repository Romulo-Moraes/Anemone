# Anemone
Anemone is a lightweight command-line argument parser for C programming language, the main idea of this argument parser is be simple, however, efficient, with positional/optional arguments, help message with program name, program version, the necessary positional arguments, the possible optional arguments and the special thanks section.


## Optional arguments
Optional arguments are arguments that are optional by default, but they can be required depending of a flag's value passed to the library. This kind of argument has two ways of be used, the normal way and the short way.

The normal way uses two dashes, a name length greater than 1 and more dashes inside the name. For instance:
```txt
--name
```

The short way uses one dash and a letter, this way support pass a block of short arguments when running the program. For instance:
```txt
-n
```

For passing a block of short arguments to a program, the user must pass one dash followed by all short arguments name that is necessary, the only thing to remember is that if one of these arguments need a value, this argument must be the last inside this argument block. For instance:
```txt
/*
	-n <value> : pass the name to the program
	-y : younger than 18
	-l : live in parents house
*/

-lyn "Bob"
```
Optional arguments can hold or not a value, if in the code definition an argument doesn't hold any value, when the user pass this flag you will only know that the flag was actived, but no values passed.

## Positional arguments
As the name says, positional arguments are caught through position in the argv array, they are always required, having no possibilities to not be. If a value in argv doesn't match as an optional argument, probably it will be a positional, but if even in this case not be, the program crash showing the help message. By the reason of the positional argument be caught by position, the user doesn't need to pass an identifier, is just a pure value; it's useful when creating softwares that always require a fixed count of input.


## Documentation
Here're the documentations of all functions available in this library, some of them have specific uses, but the great part you will always use in your programs.

### anemone_struct
anemone_struct is a struct that holds all the runtime data of the library, every function basically operates on top of it. The inner values of this struct don't must be accessed directly, there're functions that get and set values from it. This struct need to be initialized.

### initialize_lib(anemone_struct *lib, char *program_name, char *program_version, char *program_depscription, char *creators, char *special_thanks, char *email_contact);
Function that initialize the anemone_struct, this function accepts the program name, program version, program description, creators, special thanks and e-mail contact. Program name is required and need receive a string with length 1 or greater. Program version is required and need receive a string with length 1 or greater. Program description is optional, pass NULL to ignore. Creators is optional, pass NULL to ignore. Special thanks is optional, pass NULL to ignore. E-mail contact is optional, pass NULL to ignore.

### create_optional_argument(anemone_struct *lib, char *long_name, char *short_name, anemone_require_value the_argument_requires_value, anemone_optional_required argument_required);
Function that register an optional argument in the library, this function accepts long argument name, short argument name, if the argument requires a value and if the argument is required. Long argument name accepts a string with the name and format of a normal optional argument. Short argument name accepts a string with the name and format of the normal optional argument short way. Argument requires a value accepts ANEMONE_TRUE or ANEMONE_FALSE and specify if the library need to be prepared to catch a value after found this flag in the argv. Optional argument required accepts ANEMONE_TRUE or ANEMONE_FALSE and specify if the flag, even being optional, must be required. Arguments created with this function can be fetched using the long_name or short_name with other function. For instance:
```c
/* In the command line the user can 
   pass '--name bob' or even '-n bob',
   this argument is required and require a value
*/
create_optional_argument(&lib, "--name", "-n", ANEMONE_TRUE, ANEMONE_TRUE);
```

### create_positional_argument(anemone_struct *lib, char *argument_description);
Function that register a positional argument, this functions only accepts the argument description, that is useful in the help message when the user get stuck. This argument can be collectable through its index, in order of definition in the code. For instance:
```c
create_positional_argument(&lib, "The first positional argument");
```

### compile(anemone_struct *lib, int argc, char *argv[], anemone_error_status *output);
Main function of the library, after set all arguments that your program require, you can use the compile function, that does the whole process of get optional and positional arguments and give them to you using other library's functions. This function accepts the argc/argv that comes with your program by default and the error output. For instance:
```c
compile(&lib, argc, argv, &error);
```

