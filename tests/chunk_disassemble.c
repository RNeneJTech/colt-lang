#include "..colti/src/util/precomph.h"

int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		printf("Usage: "CONSOLE_FOREGROUND_BRIGHT_BLUE"colti-dis"CONSOLE_FOREGROUND_BRIGHT_MAGENTA" [path]"CONSOLE_COLOR_RESET);
		exit(3);
	}
	else
	{
		Chunk result = ChunkDeserialize(argc[1]);
		ChunkDisassemble(&result);
	}
}