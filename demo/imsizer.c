/* imsizer
 * Tom Trebisky  4-28-2025
 *
 * How much of an image is NOT 0xff empty bytes?
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>

// char *inpath = "my_demo.bin";
char *inpath = "demo.bin";

#define CHUNK	(64 * 1024)
#define MAXIM	(2*1024*1024)

unsigned char image[MAXIM];

void check_chunk ( int i, unsigned char * );

void
error ( char *msg )
{
	fprintf ( stderr, "%s\n", msg );
	exit ( 1 );
}

int
main ( int argc, char **argv )
{
		int s;
		struct stat sbuf;
		int size;
		int nn;
		int i;
		int fd;

		s = stat ( inpath, &sbuf );
		if ( s != 0 )
			error ( "No such file" );

		size = sbuf.st_size;
		printf ( "Image size = %d\n", size );

		nn = size / CHUNK;
		if ( nn * CHUNK != size )
			error ( "No tulips" );

		if ( size > MAXIM )
			error ( "Too big" );

		fd = open ( inpath, O_RDONLY );
		read ( fd, image, size );
		close ( fd );

		for ( i=0; i<nn; i++ )
			check_chunk ( i, &image[i*CHUNK] );
}

void
check_chunk ( int ii, unsigned char *chunk )
{
		int empty = 1;
		int i;

		for ( i=0; i<CHUNK; i++ )
			if ( chunk[i] != 0xff ) {
				// printf ( "   %d %02x\n", i, chunk[i] );
				empty = 0;
				break;
			}

		if ( empty )
			printf ( "Chunk %2d - empty **\n", ii );
		else
			printf ( "Chunk %2d - valid\n", ii );
}

/* THE END */
