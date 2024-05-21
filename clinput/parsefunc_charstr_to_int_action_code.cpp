#include <cstdlib>
#include <cstring>
#include "clinput.h"

ParserArgnameArgvalToCtype::errcode_t parser_charstr_to_int_action_code(
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
		if (output->data_type == ParserArgnameArgvalToCtype::DataType::INT_ACTION_CODE) {
			/*================*/
			/* parsing itself */
			//for(int i = 0; i < output_len; i++){
			if(strcmp((const char*) input->memory,"mbrread") == 0){
				*(int*) output->memory = clinput_output::ACTION_CODE_READ_DISK_MBR;
			}
			else if(strcmp((const char*) input->memory,"makediskimage") == 0){
				*(int*) output->memory = clinput_output::ACTION_CODE_MAKE_DISK_IMAGE;
			}
			else{
				*(int*) output->memory = clinput_output::ACTION_CODE_NONE;
			}

			output->memory_bytelen = (int)sizeof(int);
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
