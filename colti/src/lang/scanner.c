/** @file scanner.c
* Contains the definitions of the functions declared in 'scanner.h'
*/

#include "scanner.h"

void ScannerInit(Scanner* scan, StringView to_scan)
{
	colti_assert(scan != NULL, "Pointer was NULL!");
	memset(scan, 0, sizeof(Scanner));
	scan->view = to_scan;
}

void ScannerFree(Scanner* scan)
{
	StringFree(&scan->parsed_identifier);
}

StringView ScannerGetIdentifier(const Scanner* scan)
{
	return StringToStringView(&scan->parsed_identifier);
}
