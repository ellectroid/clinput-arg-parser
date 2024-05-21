#include "clinput.h"

ParserArgnameArgvalToCtype::errcode_t parser_cli_charstr_auto_type(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output, ParserArgnameArgvalToCtype *parser) {

	//error code is just an int
	ParserArgnameArgvalToCtype::errcode_t retval = ParserArgnameArgvalToCtype::RETVAL_OK;

	//checking if inputs are valid
	if (input == nullptr) {
		retval = ParserArgnameArgvalToCtype::RETVAL_ERROR_INPUT_NOT_SET;
		goto end;
	}
	if (output == nullptr) {
		retval = ParserArgnameArgvalToCtype::RETVAL_ERROR_OUTPUT_NOT_SET;
		goto end;
	}
	if (output->data_type == ParserArgnameArgvalToCtype::DataType::NONE) {
			retval = ParserArgnameArgvalToCtype::RETVAL_ERROR_OUTPUT_DEDUCED_TYPE_NONE;
			goto end;
	}
	if (parser == nullptr) {
		retval = ParserArgnameArgvalToCtype::RETVAL_ERROR_PARSER_NOT_SET;
		goto end;
	}

	//checking type of input, calling corresponding type handler from the vtable
	if (input->data_type == ParserArgnameArgvalToCtype::DataType::CHARSTR) {
		if(parser->parsefunc_vtable[(unsigned int)output->data_type] != nullptr){
			parser->parsefunc_vtable[(unsigned int)output->data_type](input,output);
		}
		else{
			retval = ParserArgnameArgvalToCtype::RETVAL_ERROR_OUTPUT_TYPE_PARSER_NOT_BOUND;
			goto end;
		}
	} else {
		retval = ParserArgnameArgvalToCtype::RETVAL_ERROR_INPUT_TYPE_UNSUPPORTED_BY_PARSER_FUNC;
		goto end;
	}
	end: return retval;
}
