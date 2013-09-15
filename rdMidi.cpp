//
//  rdMidi.cpp - $Revision: 1.5 $
//
//  rev 2013.09.02 gls
//

#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

//https://ccrma.stanford.edu/~craig/articles/linuxmidi/output/section1.html

int main( void )
{
   const char *device =  "/dev/midi1";
   //unsigned char data[3] = {0x90, 60, 127};

	unsigned char data[] = { 0xB0, 20, 0x31 }; // change control

   // step 1: open the OSS device for writing
   int fd = open(device, O_RDONLY, 0);
   if (fd < 0)
	{
	printf("Error: cannot open %s\n", device);
	exit(1);
	}


	int iCH = 0;
	while( 1 )
		{
		unsigned char rgbByte[3];

		int CC = read( fd, &(rgbByte[0]), sizeof(rgbByte) );

		if( (rgbByte[0] = 0xB0)
			&& (rgbByte[1] == 20) )	// Ctrl #20, Touch position
			{
			printf( "<touch val='%d'/>\n", rgbByte[ 2 ] );
			continue;
			}

		printf( "<debug CC='%d' iCH='%d' nB='0x%02X%02X%02X'/>\n"
					, CC, iCH++
					, rgbByte[0]
					, rgbByte[1]
					, rgbByte[2]
					);
		}

	while( 1 )
		{
		unsigned char nByte = 0;

		int CC = read( fd, &nByte, sizeof(nByte) );
		printf( "<debug CC='%d' iCH='%d' nB='0x%02X'/>\n"
					, CC, iCH++, nByte );
		}

   // step 2: write the MIDI information to the OSS device
   //int cWrite = write(fd, data, sizeof(data));
   close(fd);

   return 0;
}
// ////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////
