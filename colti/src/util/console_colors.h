/** @file console_colors.h
* Defines constant macros representing strings which allows to modify the font and background colors of consoles.
*/

#ifndef HG_COLTI_CONSOLE_COLORS
#define HG_COLTI_CONSOLE_COLORS

/******************************************************
FOREGROUND COLORS FOR CONSOLE
******************************************************/

#define CONSOLE_FOREGROUND_BLACK			"\x1B[30m"
#define CONSOLE_FOREGROUND_RED				"\x1B[31m"
#define CONSOLE_FOREGROUND_GREEN			"\x1B[32m"
#define CONSOLE_FOREGROUND_YELLOW			"\x1B[33m"
#define CONSOLE_FOREGROUND_BLUE				"\x1B[34m"
#define CONSOLE_FOREGROUND_MAGENTA			"\x1B[35m"
#define CONSOLE_FOREGROUND_CYAN				"\x1B[36m"
#define CONSOLE_FOREGROUND_WHITE			"\x1B[37m"

#define CONSOLE_FOREGROUND_BRIGHT_BLACK		"\x1B[90m"
#define CONSOLE_FOREGROUND_BRIGHT_RED		"\x1B[91m"
#define CONSOLE_FOREGROUND_BRIGHT_GREEN		"\x1B[92m"
#define CONSOLE_FOREGROUND_BRIGHT_YELLOW	"\x1B[93m"
#define CONSOLE_FOREGROUND_BRIGHT_BLUE		"\x1B[94m"
#define CONSOLE_FOREGROUND_BRIGHT_MAGENTA	"\x1B[95m"
#define CONSOLE_FOREGROUND_BRIGHT_CYAN		"\x1B[96m"
#define CONSOLE_FOREGROUND_BRIGHT_WHITE		"\x1B[97m"

/******************************************************
BACKGROUND COLORS FOR CONSOLE
******************************************************/

#define CONSOLE_BACKGROUND_BLACK			"\x1B[40m"
#define CONSOLE_BACKGROUND_RED				"\x1B[41m"
#define CONSOLE_BACKGROUND_GREEN			"\x1B[42m"
#define CONSOLE_BACKGROUND_YELLOW			"\x1B[43m"
#define CONSOLE_BACKGROUND_BLUE				"\x1B[44m"
#define CONSOLE_BACKGROUND_MAGENTA			"\x1B[45m"
#define CONSOLE_BACKGROUND_CYAN				"\x1B[46m"
#define CONSOLE_BACKGROUND_WHITE			"\x1B[47m"
#define CONSOLE_BACKGROUND_BRIGHT_BLACK		"\x1B[100m"
#define CONSOLE_BACKGROUND_BRIGHT_RED		"\x1B[101m"
#define CONSOLE_BACKGROUND_BRIGHT_GREEN		"\x1B[102m"
#define CONSOLE_BACKGROUND_BRIGHT_YELLOW	"\x1B[103m"
#define CONSOLE_BACKGROUND_BRIGHT_BLUE		"\x1B[104m"
#define CONSOLE_BACKGROUND_BRIGHT_MAGENTA	"\x1B[105m"
#define CONSOLE_BACKGROUND_BRIGHT_CYAN		"\x1B[106m"
#define CONSOLE_BACKGROUND_BRIGHT_WHITE		"\x1B[107m"

/******************************************************
OTHER MODIFIERS FOR CONSOLE
******************************************************/

#define CONSOLE_COLOR_RESET					"\x1B[0m"
#define CONSOLE_COLOR_REVERSE				"\x1B[2m"
#define CONSOLE_FONT_BOLD					"\x1B[2m"
#define CONSOLE_FONT_UNDERLINE				"\x1B[2m"

#endif //HG_COLTI_CONSOLE_COLORS