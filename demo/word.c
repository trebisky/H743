/* word.c
 * pull a single 32 bit word out of an image
 * Tom Trebisky
 * 4-30-2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef unsigned int u32;

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

void
show_32 ( u32 *aval )
{
		printf ( "%08x\n", *aval );
}

int
main ( int argc, char **argv )
{
		int fd;
		int n, nw;
		u32 addr;
		int off;

		if ( argc != 2 ) {
			printf ( "XXXBOGUS\n" );
			exit (1);
		}

		addr = (u32) strtol ( argv[1], NULL, 16);
		if ( addr < BASE ) {
			printf ( "XXXBOGUS\n" );
			exit (1);
		}

		// addr = 0x08000c5c;
		off = addr - BASE;

		fd = open ( inpath, O_RDONLY );
		n = read ( fd, image, MAXIM );
		close ( fd );

		// printf ( "%d bytes read\n", n );
		//nw = sizeof(int);
		// printf ( "%d words to dump\n", n/nw );

		show_32 ( (u32 *) &image[off] );
		return 0;
}

/* THE END */
