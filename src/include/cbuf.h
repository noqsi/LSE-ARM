#ifndef CBUF_H
#define CBUF_H

struct cbuf {
	char *b;		/* buffer */
	unsigned size;		/* size of buffer */
	volatile char *i;		/* input pointer */
	volatile char *o;		/* output pointer */
};

#define CBUF_FAIL 0xF00		/* impossible character */

extern int cbuf_put( struct cbuf *b, char c );
extern int cbuf_get( struct cbuf *b );
extern void cbuf_init( struct cbuf *b, char *buf, unsigned size );
extern void cbuf_clear( struct cbuf *b );

#endif /* ndef CBUF_H */

