#ifndef CLINPUT_MYAPP_IMPL_H_
#define CLINPUT_MYAPP_IMPL_H_
#include "clinput.h"

//Mandatory defines
//allocates fixed amount of memory for all arguments when parsing to arg=val
//memory-inefficient, but simple
#define CLI_ARG_VALID_MAX_COUNT		(32)
#define CLI_ARG_NAME_MAX_BYTELEN	(32)
#define CLI_ARG_VALUE_MAX_BYTELEN	(256)

int clinput(void* output_datastructure, int work_argc, char **work_argv);


//App arguments, types and output configuration
extern const char *app_argument_names[CLI_ARG_VALID_MAX_COUNT];
extern const ParserArgnameArgvalToCtype::DataType app_argument_datatypes[CLI_ARG_VALID_MAX_COUNT];
extern const int app_argument_output_size[CLI_ARG_VALID_MAX_COUNT];
extern const int app_argument_output_offset[CLI_ARG_VALID_MAX_COUNT];
extern const char app_help[];


//USERTYPE0..7 guaranteed to exist
#define CHARSTR_PATH		USERTYPE0
#define INT_ACTION_CODE		USERTYPE1


/* CHARSTR autoparser */
ParserArgnameArgvalToCtype::errcode_t parser_cli_charstr_auto_type(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output, ParserArgnameArgvalToCtype *parser);

/* vtable parsers */
//Fixed types
ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_charstr_sanitized(ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output);
ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_ull(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output);
ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_int(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output);
ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_long(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output);
ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_ll(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output);
ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_float(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output);

//User-defined types
ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_charstr_path(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output);
ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_int_action_code(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output);

/* ======== IMPORTANT ======== */
//output data structure
//you should instead include header with required data type into this file
//in that case you don't need a struct here
//this is just to make an example work
//#include "outputtype.h"
typedef struct clinput_output{
	enum ActionCode{
		ACTION_CODE_NONE,
		ACTION_CODE_READ_DISK_MBR = 0x10,
		ACTION_CODE_MAKE_DISK_IMAGE = 0x20,
	};
	char input_file_path[32];
	int action_code;
	int verbosity_level;
}clinput_output;
#endif /* CLINPUT_MYAPP_IMPL_H_ */
