#ifndef CLINPUT_PARSER_TO_CTYPE_H_
#define CLINPUT_PARSER_TO_CTYPE_H_
#include <cstring>

class ParserArgnameArgvalToCtype {
public:
	typedef int errcode_t;
	friend class OutputTypeTable;

	enum RETVAL {
		RETVAL_OK,
		RETVAL_ERROR_BAD_VTABLE,
		RETVAL_ERROR_NO_PARSER,
		RETVAL_ERROR_INPUT_TYPE_UNSUPPORTED_BY_PARSER_FUNC,
		RETVAL_ERROR_OUTPUT_TYPE_UNSUPPORTED_BY_PARSER_FUNC,
		RETVAL_ERROR_INPUT_NOT_SET,
		RETVAL_ERROR_OUTPUT_NOT_SET,
		RETVAL_ERROR_PARSER_NOT_SET,
		RETVAL_ERROR_OUTPUT_TYPE_TABLE_NOT_SET,
		RETVAL_ERROR_OUTPUT_TYPE_TABLE_FULL,
		RETVAL_ERROR_OUTPUT_TYPE_PARSER_NOT_BOUND,
		RETVAL_ERROR_OUTPUT_DEDUCED_TYPE_NONE,
		RETVAL_ERROR_PARSING_ABORTED_AFTER_ARGNAME_PROCESSING,
	};

	enum class DataType {
		NONE,
		CHAR,
		UCHAR,
		SHORT,
		USHORT,
		INT,
		UINT,
		LONG,
		ULONG,
		LONGLONG,
		ULONGLONG,
		INT16,
		UINT16,
		INT32,
		UINT32,
		INT64,
		UINT64,
		RESERVED0,
		RESERVED1,
		RESERVED2,
		RESERVED3,
		FLOAT,
		DOUBLE,
		POINTER,
		CHARSTR,
		HEXCHARSTR,
		WCHAR16,
		WCHAR16STR,
		CHAR32,
		WCHAR32STR,
		RESERVED4,
		RESERVED5,
		USERTYPE0,
		USERTYPE1,
		USERTYPE2,
		USERTYPE3,
		USERTYPE4,
		USERTYPE5,
		USERTYPE6,
		USERTYPE7,
		TYPECOUNT
	};
	typedef struct ParserInput {
		DataType data_type;
		void *memory;
		unsigned int memory_bytelen;
	} ParserInput;

	typedef struct ParserOutput {
		DataType data_type;
		void *memory;
		unsigned int memory_bytelen;
	} ParserOutput;

	typedef errcode_t (*parse_func_t)(ParserInput *input, ParserOutput *output);

	parse_func_t *parsefunc_vtable;
	unsigned int parsefunc_vtable_len;

	errcode_t parse(unsigned int parsefunc_id, ParserInput *input,
			ParserOutput *output) {
		int retval = RETVAL_OK;
		if ((parsefunc_vtable == nullptr) || (parsefunc_vtable_len == 0)) {
			retval = RETVAL_ERROR_BAD_VTABLE;
			goto end;
		}
		if (parsefunc_vtable[parsefunc_id] != nullptr) {
			retval = parsefunc_vtable[parsefunc_id](input, output);
			goto end;
		} else {
			retval = RETVAL_ERROR_NO_PARSER;
			goto end;
		}
		end: return retval;
	}

};

class ParserArgnameCtypeOutputTypeMemoryTable {
public:
	friend class TypeParser;
	int entry_count;
	char **argname;
	ParserArgnameArgvalToCtype::DataType *argtype;
	void **output_memory;
	int *output_memory_bytelen;
	int entry_allocate(int count) {
		if (count < 1)
			return 0;
		char **argnamebuf = new char*[count]();
		ParserArgnameArgvalToCtype::DataType *argtypebuf =
				new ParserArgnameArgvalToCtype::DataType[count]();
		void **outputmemorybuf = new void*[count]();
		int *outputmemory_size = new int[count]();
		argname = argnamebuf;
		argtype = argtypebuf;
		output_memory = outputmemorybuf;
		output_memory_bytelen = outputmemory_size;
		entry_count = count;
		return count;
	}
	int entry_push(const char *cli_argument,
			ParserArgnameArgvalToCtype::DataType parse_type, void *output_mem,
			int output_mem_bytelen) {
		int retval_new_elem_index = -1;
		int first_empty_search_index;
		for (first_empty_search_index = 0;
				first_empty_search_index < entry_count;
				first_empty_search_index++) {
			if (argname[first_empty_search_index] == nullptr)
				break;
		}
		if (first_empty_search_index == entry_count)
			goto end;
		retval_new_elem_index = first_empty_search_index;
		argname[first_empty_search_index] = (char*) cli_argument;
		argtype[first_empty_search_index] = parse_type;
		output_memory[first_empty_search_index] = output_mem;
		output_memory_bytelen[first_empty_search_index] = output_mem_bytelen;
		end: return retval_new_elem_index;
	}

	ParserArgnameArgvalToCtype::DataType output_type_get(
			const char *input_argname) {
		ParserArgnameArgvalToCtype::DataType retval =
				ParserArgnameArgvalToCtype::DataType::NONE;
		int entry = 0;
		if (input_argname[0] == 0x00)
			goto end;
		for (; entry < entry_count; entry++) {
			if (argname[entry] == nullptr)
				goto end;
			if (strcmp(input_argname, argname[entry]) == 0) {
				retval = argtype[entry];
				break;
			}
		}
		end: return retval;
	}

	void* output_memory_get(const char *input_argname) {
		void *retval = nullptr;
		int entry = 0;
		if (input_argname[0] == 0x00)
			goto end;
		for (; entry < entry_count; entry++) {
			if (argname[entry] == nullptr) continue;
			if (strcmp(input_argname, argname[entry]) == 0) {
				retval = output_memory[entry];
				break;
			}
		}
		end: return retval;
	}

	int output_memory_size_get(const char *input_argname) {
		int retval = 0;
		int entry = 0;
		if (input_argname[0] == 0x00)
			goto end;
		for (; entry < entry_count; entry++) {
			if (argname[entry] == nullptr) continue;
			if (strcmp(input_argname, argname[entry]) == 0) {
				retval = output_memory_bytelen[entry];
				break;
			}
		}
		end: return retval;
	}

};

#endif /* CLINPUT_PARSER_TO_CTYPE_H_ */
