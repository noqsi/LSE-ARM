#ifndef CBUF_H
#define CBUF_H

struct cbuf {
	char *b;		/* buffer */
	unsigned size;		/* size of buffer */
	char *i;		/* input pointer */
	char *o;		/* output pointer */
};

#define CBUF_FAIL 0xF00		/* impossible character */

extern int cbuf_put( struct cbuf *b, char c );
extern int cbuf_get( struct cbuf *b );
extern void cbuf_init( struct cbuf *b, char *buf, unsigned size );

#endif /* ndef CBUF_H */

