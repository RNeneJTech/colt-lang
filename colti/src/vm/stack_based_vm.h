#ifndef HG_COLTI_STACK_BASED_VM
#define HG_COLTI_STACK_BASED_VM

#include "common.h"

#include "byte-code/chunk.h"
#include "values/colti_floating_value.h"

typedef struct
{
	QWORD* stack_top;
	QWORD stack[256];
} StackVM;

void StackVMInit(StackVM* vm);

void StackVMFree(StackVM* vm);

void StackVMPush(StackVM* vm, QWORD value);

QWORD StackVMTop(StackVM* vm);

QWORD StackVMPop(StackVM* vm);

bool StackVMIsEmpty(const StackVM* vm);

uint64_t StackVMSize(const StackVM* vm);

InterpretResult StackVMRun(StackVM* vm, Chunk* chunk);

#endif //HG_COLTI_STACK_BASED_VM