#include <iostream>
#include "clinput.h"

extern ParserArgnameArgvalToCtype::parse_func_t parse_func_vtable[(unsigned long long) ParserArgnameArgvalToCtype::DataType::TYPECOUNT];

static int parse_raw_input_to_name_value(int work_argc, char **work_argv,
		ParserOutputArgNameArgVal *arg_raw);
static int parse_vtable_parser_bind(ParserArgnameArgvalToCtype *parser);
static int output_type_table_fill(
		ParserArgnameCtypeOutputTypeMemoryTable *output_type_table,
		void *output_memstart);
static int parse(int work_argc, char **work_argv,
		ParserOutputArgNameArgVal *arg_raw, ParserArgnameArgvalToCtype *parser,
		ParserArgnameCtypeOutputTypeMemoryTable *output_type_table);

//This is the only function called by main(int argc, char** argv);
//It takes arg strings, breaks them down into name=value pairs
//Then parser is created with type-associated vector table with parsers bound to various types
//Then output memory & type table is created. It contains argument names,
//each associated with a specific type, which will invoke the same parser function
//for all inputs of the same desired type.
int clinput(void *output_datastructure, int work_argc, char **work_argv) {
	int retval = ParserArgnameArgvalToCtype::RETVAL_OK;
	ParserOutputArgNameArgVal arg_raw = { };
	ParserArgnameArgvalToCtype parser = { };
	ParserArgnameCtypeOutputTypeMemoryTable output_type_table = { };
	retval = parse_raw_input_to_name_value(work_argc, work_argv, &arg_raw);
	if (retval != ParserArgnameArgvalToCtype::RETVAL_OK)
		goto end;

	/* THIS IS WHERE YOU WANT TO PROCESS VALUELESS ARGS (such as app name, flags) */
	//if somebody entered app.exe help ...
	for (int i = 0; i < arg_raw.argument_count; i++) {
		if (strcmp(arg_raw.argname[i], "help") == 0) {
			if (i != 0) {
				std::cout << app_help << std::endl;
				retval =
						ParserArgnameArgvalToCtype::RETVAL_ERROR_PARSING_ABORTED_AFTER_ARGNAME_PROCESSING;
				goto end;
			}

		}
	}


	retval = parse_vtable_parser_bind(&parser);
	if (retval != ParserArgnameArgvalToCtype::RETVAL_OK)
		goto end;
	retval = output_type_table_fill(&output_type_table, output_datastructure);
	if (retval != ParserArgnameArgvalToCtype::RETVAL_OK)
		goto end;
	retval = parse(work_argc, work_argv, &arg_raw, &parser, &output_type_table);
	if (retval != ParserArgnameArgvalToCtype::RETVAL_OK)
		goto end;

	end: delete[] arg_raw.argname;
	delete[] arg_raw.argval;
	delete[] parser.parsefunc_vtable;
	delete[] output_type_table.argname;
	delete[] output_type_table.argtype;
	delete[] output_type_table.output_memory;
	return retval;
}

//This is the first stage of parsing. Converting an argument string
//into two strings as name=val, the symbol "=" is excluded from both
//if there is no "=", then the whole argument is argname
static int parse_raw_input_to_name_value(int work_argc, char **work_argv,
		ParserOutputArgNameArgVal *arg_raw) {
	arg_raw->argument_count = work_argc;
	arg_raw->argname = new char*[work_argc];
	arg_raw->argval = new char*[work_argc];

	for (int i = 0; i < work_argc; i++) {
		arg_raw->argname[i] = new char[CLI_ARG_NAME_MAX_BYTELEN]();
	}
	for (int i = 0; i < work_argc; i++) {
		arg_raw->argval[i] = new char[CLI_ARG_VALUE_MAX_BYTELEN]();
	}
	for (int i = 0; i < work_argc; i++) {
		ParserCharStrToArgNameArgVal arg = ParserCharStrToArgNameArgVal(
				work_argv[i]);
		arg.name_write(arg_raw->argname[i], CLI_ARG_NAME_MAX_BYTELEN);
		arg.value_write(arg_raw->argval[i], CLI_ARG_VALUE_MAX_BYTELEN);
	}
	return 0;
}

//creating parser object, which will contain the vector table and use the output type table (below)
//it binds individual parser functions and associates them with types
static int parse_vtable_parser_bind(ParserArgnameArgvalToCtype *parser) {
	parser->parsefunc_vtable_len = sizeof(parse_func_vtable) / sizeof(void*);
	parser->parsefunc_vtable =
			new ParserArgnameArgvalToCtype::parse_func_t[parser->parsefunc_vtable_len]();
	for (unsigned int i = 0; i < parser->parsefunc_vtable_len; i++) {
		parser->parsefunc_vtable[i] = parse_func_vtable[i];
	}
	return 0;
}

//creating output type table, which contains output memory
//and output data type the input is supposed to be parsed as
//this is where you define that myint=15 has to be parsed as integer
static int output_type_table_fill(
		ParserArgnameCtypeOutputTypeMemoryTable *output_type_table,
		void *output_memstart) {
	int retval = ParserArgnameArgvalToCtype::RETVAL_OK;
	int entry_position = -1;
	output_type_table->entry_allocate(CLI_ARG_VALID_MAX_COUNT);

	for (int i = 0; i < CLI_ARG_VALID_MAX_COUNT; i++) {
		entry_position = output_type_table->entry_push(app_argument_names[i],
				app_argument_datatypes[i],
				(char*) output_memstart + app_argument_output_offset[i], app_argument_output_size[i]);
		if (entry_position < 0)
			goto error_full;
	}

	goto end;
	error_full: retval =
			ParserArgnameArgvalToCtype::RETVAL_ERROR_OUTPUT_TYPE_TABLE_FULL;
	end: return retval;
}

//This function takes argname & argval values and gets argument type and output memory,
//configures parser output data type and output memory, and calls the parser
static int parse(int work_argc, char **work_argv,
		ParserOutputArgNameArgVal *arg_raw, ParserArgnameArgvalToCtype *parser,
		ParserArgnameCtypeOutputTypeMemoryTable *output_type_table) {
	int retval = 0;
	for (int arg_index = 0; arg_index < work_argc; arg_index++) {
		ParserArgnameArgvalToCtype::ParserInput parser_input_charstr_argval =
				{ };
		parser_input_charstr_argval.memory = (void*) arg_raw->argval[arg_index];
		parser_input_charstr_argval.memory_bytelen = strlen((const char*) arg_raw->argval[arg_index]) + 1;
		parser_input_charstr_argval.data_type =
				ParserArgnameArgvalToCtype::DataType::CHARSTR;

		ParserArgnameArgvalToCtype::ParserOutput parser_output_charstr_argval =
				{ };
		parser_output_charstr_argval.memory =
				output_type_table->output_memory_get(
						(const char*) arg_raw->argname[arg_index]);
		parser_output_charstr_argval.memory_bytelen = output_type_table->output_memory_size_get((const char*) arg_raw->argname[arg_index]);
		parser_output_charstr_argval.data_type =
				output_type_table->output_type_get(
						(const char*) arg_raw->argname[arg_index]);

		int parse_return_code = parser_cli_charstr_auto_type(
				&parser_input_charstr_argval, &parser_output_charstr_argval,
				parser);
		retval = parse_return_code;
		if (parse_return_code != ParserArgnameArgvalToCtype::RETVAL_OK) {
			if ((arg_index == 0)
					&& parse_return_code
							== ParserArgnameArgvalToCtype::RETVAL_ERROR_OUTPUT_DEDUCED_TYPE_NONE) {
				retval = ParserArgnameArgvalToCtype::RETVAL_OK;
			}
			if (arg_index != 0) {
				if (parse_return_code
						== ParserArgnameArgvalToCtype::RETVAL_ERROR_OUTPUT_TYPE_PARSER_NOT_BOUND) {
					goto end;
				}

				if (parse_return_code
						== ParserArgnameArgvalToCtype::RETVAL_ERROR_OUTPUT_DEDUCED_TYPE_NONE) {
					std::cout << "Error. Unknown argument number " << arg_index
							<< std::endl;
					goto end;
				}

			}
		}
	}
	end: return retval;
}

