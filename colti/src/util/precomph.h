/** @file precomph.h
* Precompiled header file containing (mostly) static includes
*/

#ifndef HG_COLTI_PRECOMPH
#define HG_COLTI_PRECOMPH

// NON-STD INCLUDES
#include "common.h"
#include "byte_code.h"
#include "disassemble.h"
#include "chunk.h"

#include "lang/scanner.h"

//VMs
#include "vm/stack_based_vm.h"

//UTILITIES
#include "structs/struct_string.h"
#include "util/parse_args.h"

//DEBUGING UTILITIES
#if defined(COLTI_WINDOWS) && defined(COLTI_DEBUG_BUILD)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h> //Contains _CrtDumpMemoryLeaks()
	#define DUMP_MEMORY_LEAKS() _CrtDumpMemoryLeaks()
#else
	/// @brief On Windows and Debug build, using Visual Studio, this will show memory leaks (if there are any)
	#define DUMP_MEMORY_LEAKS() do {} while(0)
#endif

#endif //HG_COLTI_PRECOMPH