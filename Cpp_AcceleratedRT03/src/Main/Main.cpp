#include <iostream>
#include <SDL.h>

#include "../Programs/Program.h"

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	Program p = Program();
	p.loop();

	return EXIT_SUCCESS;
}