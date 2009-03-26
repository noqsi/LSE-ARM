#include <stdio.h>

char app_lse[] = "1 doPrompt !\n";

char readchar( void ){ return getchar(); }

void writechar( char c ){ putchar( c ); }

main()
{
	lse_init();
	lse_main();
}
