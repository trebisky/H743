/* longs.c
 * dump an image as a bunch of 32 bit longs
 * Tom Trebisky
 * 4-30-2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

char *inpath = "demo.bin";

#define MAXIM	(1*1024*1024)
// #define MAXIM	(2*1024*1024)
unsigned char image[MAXIM];

#define BASE 0x08000000

void
error ( char *msg )
{
	fprintf ( stderr, "%s\n", msg );
	exit ( 1 );
}

void dumper ( int *buf, int count );

int
main ( int argc, char **argv )
{
		int fd;
		int n, nw;

		fd = open ( inpath, O_RDONLY );
		n = read ( fd, image, MAXIM );
		close ( fd );

		printf ( "%d bytes read\n", n );

		nw = sizeof(int);

		printf ( "%d words to dump\n", n/nw );

		dumper ( (int *) image, n/nw );
}

#define BASE 0x08000000

void
dumper ( int *buf, int count )
{
		int i;
		unsigned int addr = BASE;

		for ( i=0; i<count; i++ ) {
			printf ( "%08x:  %08x\n", addr, buf[i] );
			addr += 4;
		}
}

/* THE END */
