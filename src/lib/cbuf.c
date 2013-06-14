#include "cbuf.h"

int cbuf_put( struct cbuf *b, char c )
{
	volatile char *new = b->o + 1;
	if( new >= b->b + b->size ) new = b->b;	/* wrap */
	if( new == b->i ) return CBUF_FAIL;	/* full */
	*b->o = c;
	b->o = new;
	return 0;				/* success */
}

int cbuf_get( struct cbuf *b )
{
	volatile char *new;
	char c;
	if( b->i == b->o ) return CBUF_FAIL;	/* empty */
	c = *b->i;
	new = b->i + 1;
	if( new >= b->b + b->size ) new = b->b;
	b->i = new;
	return c;
}
	
void cbuf_init( struct cbuf *b, char *buf, unsigned size )
{
	b->i = b->o = b->b = buf;
	b->size = size;
}

void cbuf_clear( struct cbuf *b )
{
	b->i = b->o;
}
