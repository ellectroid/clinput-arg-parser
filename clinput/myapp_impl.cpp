#include <cstddef>
#include "myapp_impl.h"

#define member_size(type, member) (sizeof( ((type *)0)->member ))

/* I'm going to parse different types, so I'm initializing vtable with implementations.
 * Each type has its fixed place. Check out ParserArgnameArgvalToCtype::DataType.
 * There are a few reserved & user-defined slots for arbitrary types, a couple of which I also use,
 * so you see how I can create my own type in a minute.*/
ParserArgnameArgvalToCtype::parse_func_t parse_func_vtable[(unsigned long long) ParserArgnameArgvalToCtype::DataType::TYPECOUNT] =
		{ nullptr, //[0]
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				parser_charstr_to_int,
				nullptr,
				nullptr,
				nullptr,
				parser_charstr_to_ll,
				parser_charstr_to_ull, //[10]
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr, //[20]
				parser_charstr_to_float,
				nullptr,
				nullptr,
				parser_charstr_to_charstr_sanitized,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr, //[30]
				nullptr,
				//USERTYPE START
				parser_charstr_to_charstr_path,
				parser_charstr_to_int_action_code,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr };

const char *app_argument_names[CLI_ARG_VALID_MAX_COUNT] = {
		"action",
		"inputpath",
		"verbosity-level" };

//my arguments' types
const ParserArgnameArgvalToCtype::DataType app_argument_datatypes[CLI_ARG_VALID_MAX_COUNT] =
		{
				ParserArgnameArgvalToCtype::DataType::INT_ACTION_CODE,
				ParserArgnameArgvalToCtype::DataType::CHARSTR_PATH,
				ParserArgnameArgvalToCtype::DataType::INT };

const int app_argument_output_size[CLI_ARG_VALID_MAX_COUNT] = {
		sizeof(int),
		member_size(clinput_output,input_file_path),
		sizeof(int)
};

//their output offset inside the structure
const int app_argument_output_offset[CLI_ARG_VALID_MAX_COUNT] = {
		offsetof(clinput_output, action_code),
		offsetof(clinput_output, input_file_path),
		offsetof(clinput_output, verbosity_level)
};

const char app_help[] =
		"This is a help message.";

