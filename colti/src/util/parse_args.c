#include "parse_args.h"

ParseResult ParseArguments(int argc, const char** argv)
{
	ParseResult result;
	memset(&result, 0, sizeof(ParseResult));
	
	for (size_t i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			CommandLineArgument arg = impl_string_to_arg(argv[i]);
			switch (arg)
			{
				//Even though all the impl_{ARG} do not return, break to silence warnings
			break; case ARG_HELP:
				impl_help(argc, argv);
			break; case ARG_VERSION:
				impl_version(argc, argv);
			break; case ARG_DISASSEMBLE:
				impl_disassemble(argc, argv);
			break; case ARG_EXEC_OUTPUT:
				//As the function will read 1 argument more, we need to update i
				result.file_path_out = impl_exec_out(argc, argv, ++i);
			break; case ARG_BYTE_CODE_OUTPUT:
				//As the function will read 1 argument more, we need to update i
				result.byte_code_out = impl_byte_out(argc, argv, ++i);
			break; default:
				printf(CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"Unknown argument! Use '-e' or '--enum' to get the list of valid arguments.\n");
				exit(3);
			}
		}
		else
		{
			result.file_path_in = argv[i];
		}
	}
	//If the user passed an -o or -b, an input file SHOULD BE SPECIFIED
	if ((result.file_path_out != NULL || result.byte_code_out != NULL) && result.file_path_in == NULL)
	{
		printf(CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"No input file!\n");
		exit(3);
	}
	//If the user did not pass an -o, choose a default output
	if (result.file_path_out == NULL && result.file_path_in != NULL)
	{
		result.file_path_out = "a.out"; //Keeping the tradition alive
	}	
	return result;
}

bool checkIfValidFile(const char* path)
{
	FILE* file = fopen(path, "rb");
	if (file == NULL)
		return false;
	fclose(file);
	return true;
}

/*****************************************
IMPLEMENTATION HELPERS
*****************************************/

CommandLineArgument impl_string_to_arg(const char* str)
{
	colti_assert(strlen(str) > 1, "String size should be greater than 1!");
	size_t length = strlen(str);
	if (str[0] != '-')
		return ARG_INVALID;
	else if (length == 2)
	{
		switch (str[length - 1])
		{
		case 'v':
			return ARG_VERSION;
		case 'h':
			return ARG_HELP;
		case 'd':
			return ARG_DISASSEMBLE;
		case 'o':
			return ARG_EXEC_OUTPUT;
		case 'b':
			return ARG_BYTE_CODE_OUTPUT;
		default:
			return ARG_INVALID;
		}
	}
	else if (str[1] == '-') // --...
	{
		//We optimize comparing string by first checking for the first character after --
		switch (str[2])
		{
		case 'v':
			if (strcmp(str + 3, "ersion") == 0) //we already checked for --v
				return ARG_VERSION;
			return ARG_INVALID;
		case 'h':
			if (strcmp(str + 3, "elp") == 0) //we already checked for --h
				return ARG_HELP;
			return ARG_INVALID;
		case 'd':
			if (strcmp(str + 3, "isassemble") == 0)
				return ARG_DISASSEMBLE;
			return ARG_INVALID;
		case 'o':
			if (strcmp(str + 3, "ut") == 0)
				return ARG_EXEC_OUTPUT;
			return ARG_INVALID;
		case 'b':
			if (strcmp(str + 3, "yte-out") == 0)
				return ARG_BYTE_CODE_OUTPUT;
			return ARG_INVALID;
		default:
			return ARG_INVALID;
		}
	}
	return ARG_INVALID;
}

void impl_version(int argc, const char** argv)
{
	if (argc == 2)
	{
		printf(CONSOLE_BACKGROUND_BRIGHT_MAGENTA CONSOLE_FOREGROUND_BLACK
			"COLTI v%s ("COLTI_CONFIG_STRING") on %s" CONSOLE_COLOR_RESET "\n", COLTI_VERSION_STRING, COLTI_OS_STRING);
		exit(0);
	}
	else
	{
		impl_print_invalid_combination(argc, argv);
		exit(3);
	}
}

void impl_disassemble(int argc, const char** argv)
{
	if (argc == 3)
	{
		if (checkIfValidFile(argv[2]))
		{
			Chunk chunk = ChunkDeserialize(argv[2]);
			ChunkDisassemble(&chunk, argv[2]);
			ChunkFree(&chunk);
			exit(0);
		}
		else
		{
			printf(CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"'%s' is not a valid path!\n", argv[2]);
			exit(3);
		}
	}
	else if (argc == 2)
	{
		impl_help_disassemble();
		exit(3);
	}
	else
	{
		impl_print_invalid_combination(argc, argv);
		exit(3);
	}
}

void impl_help(int argc, const char** argv)
{
	if (argc == 2)
	{
		impl_help_help();
		exit(3);
	}
	else if (argc == 3)
	{
		CommandLineArgument arg = impl_string_to_arg(argv[2]);
		switch (arg)
		{
		case ARG_HELP:
			impl_help_help();
			exit(0);
		case ARG_DISASSEMBLE:
			impl_help_disassemble();
			exit(0);
		case ARG_VERSION:
			impl_help_version();
			exit(0);
		case ARG_EXEC_OUTPUT:
			impl_help_exec_out();
			exit(0);
		case ARG_BYTE_CODE_OUTPUT:
			impl_help_byte_out();
			exit(0);
		default:
			impl_print_invalid_combination(argc, argv);
			exit(3);
		}
	}
	else
	{
		impl_print_invalid_combination(argc, argv);
		exit(3);
	}
}

const char* impl_exec_out(int argc, const char** argv, size_t current_argc)
{
	if (current_argc == argc)
	{
		printf(CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"Expected a file path!\n");
		exit(3);
	}
	if (impl_string_to_arg(argv[current_argc]) == ARG_INVALID)
		return argv[current_argc];
	else
	{
		printf(CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"Expected a file path, not '%s'!\n", argv[current_argc]);
		exit(3);
	}
}

const char* impl_byte_out(int argc, const char** argv, size_t current_argc)
{
	if (current_argc == argc)
	{
		printf(CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"Expected a file path!\n");
		exit(3);
	}
	if (impl_string_to_arg(argv[current_argc]) == ARG_INVALID)
		return argv[current_argc];
	else
	{
		printf(CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"Expected a file path, not '%s'!\n", argv[current_argc]);
		exit(3);
	}
}

void impl_print_invalid_combination(int argc, const char** argv)
{
	colti_assert(argc >= 2, "Expected 'argc' greater or equal to 2!");
	printf(CONSOLE_FOREGROUND_BRIGHT_RED"Error: "CONSOLE_COLOR_RESET"Invalid argument combination for '%s'", argv[1]);
	if (argc > 2)
	{
		fputc(':', stdout);
		for (size_t i = 2; i < argc; i++)
		{
			printf(" %s", argv[i]);
		}
	}
	fputs(".\n", stdout);
}

void impl_help_disassemble()
{
	printf(CONSOLE_FOREGROUND_BRIGHT_CYAN"-d, --disassemble"CONSOLE_COLOR_RESET": Disassembles a serialized chunk of code (compiled byte-code), which usually ends with '.ctc'.\nUse: "CONSOLE_FOREGROUND_BRIGHT_CYAN"--disassemble"CONSOLE_FOREGROUND_BRIGHT_MAGENTA" <PATH>\n"CONSOLE_COLOR_RESET);
}

void impl_help_version()
{
	printf(CONSOLE_FOREGROUND_BRIGHT_CYAN"-v, --version"CONSOLE_COLOR_RESET": Prints the version of the compiler.\nFormat: "CONSOLE_FOREGROUND_BRIGHT_CYAN"--version\n"CONSOLE_COLOR_RESET);
}

void impl_help_help()
{
	printf(CONSOLE_FOREGROUND_BRIGHT_CYAN"-h, --help"CONSOLE_COLOR_RESET": Prints the purpose and use of an argument.\nFormat: "CONSOLE_FOREGROUND_BRIGHT_CYAN"--help"CONSOLE_FOREGROUND_BRIGHT_MAGENTA" <ARG>\n"CONSOLE_COLOR_RESET);
}

void impl_help_exec_out()
{
	printf(CONSOLE_FOREGROUND_BRIGHT_CYAN"-o, --out"CONSOLE_COLOR_RESET": Specifies the executable output path.\nUse: "CONSOLE_FOREGROUND_BRIGHT_CYAN"--out"CONSOLE_FOREGROUND_BRIGHT_MAGENTA" <PATH>\n"CONSOLE_COLOR_RESET);
}

void impl_help_byte_out()
{
	printf(CONSOLE_FOREGROUND_BRIGHT_CYAN"-b, --byte-out"CONSOLE_COLOR_RESET": Specifies the byte-code output path.\nUse: "CONSOLE_FOREGROUND_BRIGHT_CYAN"--byte-out"CONSOLE_FOREGROUND_BRIGHT_MAGENTA" <PATH>\n"CONSOLE_COLOR_RESET);
}
