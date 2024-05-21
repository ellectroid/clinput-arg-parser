#include <iostream>
#include "clinput/clinput.h"

#define IMITATE_CONSOLE_INPUT	1 //0 = use console args

const char *fake_argv[] = {
		"app.exe",
		"action=makediskimage",
		"inputpath=\\\\.\\PhysicalDrive0",
		"verbosity-level=2", };
const int fake_argc = sizeof(fake_argv) / sizeof(fake_argv[0]);

int main(int argc, char **argv) {
	int parse_retval = 0;
	clinput_output parse_result = { };
#if(IMITATE_CONSOLE_INPUT == 1)
	parse_retval = clinput(&parse_result, fake_argc, (char**) fake_argv);
#else
	parse_retval = clinput(&parse_result, argc, argv);
#endif
	std::cout << "Parse return value: " << parse_retval << std::endl;
	std::cout << std::endl;
	if (parse_retval == 0) {
		std::cout << "Printing contents of the output structure: " << std::endl;
		std::cout << "action=" << +parse_result.action_code << std::endl;
		std::cout << "verbosity-level=" << +parse_result.verbosity_level
				<< std::endl;
		std::cout << "inputpath=" << parse_result.input_file_path << std::endl;

	}
	return parse_retval;
}
