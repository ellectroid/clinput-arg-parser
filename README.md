This is a command line argument parser.

This is a demo project.

Example:
Console input: 
>app.exe action=makediskimage inputpath=\\.\PhysicalDrive0 verbosity-level=2

Clinput output (configurable):
RAM contains
struct clinput_output{
	char input_file_path[32];
	int action_code;
	int verbosity_level;
}

(can freely move members of struct around in the definition, resize the array and it won't break anything!!!)

Key points:
-Simplicity of use: easy to configure for your app's argument names and their parse types (see "impl" files)
-Output data structure from the console input arguments is fully user-defined. No limits.
-Modularity: can hook up another parser function for any type, user-defined types
-Internal modularity: data is processed in multiple stages, so things can be easily adjusted, improved
-Error checking: functions return error codes
-Handles only the arguments provided, if there is an unknown argument, throws an error, tells what argument wasn't recognized
-It doesn't care about specific output types. You can easily specify your own type with any memory layout and your own parser behavior (which is the point)
-Each parser is just a single function with a fixed signature, the parser is provided with input data and output type information. The parser function can do anything.
-A couple basic parsers included (for output types argument value charstr to charstr, to int, to float, unsigned long long, signed long long)
-An example that takes advantage of modularity and customizability by using a default integer parser, as well as user-defined types, and a custom argument value string to integer parser

How it works:
When you pass arguments, they're a bunch of strings "app.exe" "argstring1" "argstring2" "argstring3".
Stage 1 calls "raw argument to argname=argval". It finds "=" and breaks the argument apart into separate argument name strings and value strings.
Stage 2 binds parser functions to types (a vtable).
Stage 3 binds user-defined argument names to types, output memory structure
Stage 4 parser takes output of stage 1 (argname=argval), goes through every argument, looks up its type and invokes the corresponding parser
