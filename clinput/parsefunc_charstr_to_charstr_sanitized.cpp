#include <cstdlib>

#include "clinput.h"

ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_charstr_sanitized(
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
		if (output->data_type == ParserArgnameArgvalToCtype::DataType::CHARSTR) {
			/*================*/
			/* parsing itself */
			unsigned int output_len = output->memory_bytelen;
			if ((input->memory_bytelen > 0) && (input->memory_bytelen < output_len))
				output_len = input->memory_bytelen;

			for (unsigned int i = 0; i < output_len; i++) {
				/* SANITIZE INPUT HERE */
				if (/*((char*) input->memory)[i] == 'm'*/0) { //filter out undesirable characters
					((char*) output->memory)[i] = '/'; //replaced with something else or do other sanitation
				} else {
					((char*) output->memory)[i] = ((char*) input->memory)[i];
				}
				if (((char*) input->memory)[i] == 0x00){
					output_len = i;
					break;
				}

			}
			((char*) output->memory)[output_len - 1] = 0x00;
			output->memory_bytelen = output_len;
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
