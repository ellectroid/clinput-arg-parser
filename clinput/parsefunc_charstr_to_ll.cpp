#include <cstdlib>

#include "clinput.h"

ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_ll(
		ParserArgnameArgvalToCtype::ParserInput *input,
		ParserArgnameArgvalToCtype::ParserOutput *output) {

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

	//checking types of input and output
	//if ok, actually parsing
	if (input->data_type == ParserArgnameArgvalToCtype::DataType::CHARSTR) {
		if (output->data_type == ParserArgnameArgvalToCtype::DataType::LONGLONG) {
			/*================*/
			/* parsing itself */
			//for(int i = 0; i < output_len; i++){
			*(unsigned long long*) output->memory = strtoll(
					(char*) input->memory, nullptr, 10);
			output->memory_bytelen = (int)sizeof(long long);
			/* end of parsing */
			/*================*/
		} else {
			retval =
					ParserArgnameArgvalToCtype::RETVAL_ERROR_OUTPUT_TYPE_UNSUPPORTED_BY_PARSER_FUNC;
			goto end;
		}
	} else {
		retval = ParserArgnameArgvalToCtype::RETVAL_ERROR_INPUT_TYPE_UNSUPPORTED_BY_PARSER_FUNC;
		goto end;
	}
	end: return retval;
}
