//
//  wrMidi.cpp - $Revision: 1.9 $
//
//  rev 2013.09.03 gls continue
//  rev 2013.09.02 gls
//
#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//https://ccrma.stanford.edu/~craig/articles/linuxmidi/output/section1.html

int main( int cArg, char *rgpszArg[] )
{
   	const char *device =  "/dev/midi1";

   //unsigned char rgbData[3] = {0x90, 60, 127};
	unsigned char rgbData[] = { 0xB0, 20, 0x31 }; // change control

	for(int iA=0; iA<cArg; iA++)
		{
		char *pszArg = rgpszArg[ iA ];

		if( !strcmp( pszArg, "--val" ) )
			{
			rgbData[ 2 ] = atoi( rgpszArg[ ++iA ] );
			}
		else if( !strcmp( pszArg, "--dev1" ) )
			{
            device = "/dev/midi1";
            }
		else if( !strcmp( pszArg, "--dev2" ) )
			{
            device = "/dev/midi2";
            }
		else if( !strcmp( pszArg, "--led1" ) )
			{
            rgbData[ 0 ] = 0xAD;
			unsigned int nVal = atoi( rgpszArg[ ++iA ] );
			rgbData[ 1 ] = 0x7F & nVal; // data bits 0:6
			rgbData[ 2 ] = 0x7F & (nVal>>7);
            }
		else if( !strcmp( pszArg, "--led2" ) )
			{
            rgbData[ 0 ] = 0xAE;
			unsigned int nVal = atoi( rgpszArg[ ++iA ] );
			rgbData[ 1 ] = 0x7F & nVal; // data bits 0:6
			rgbData[ 2 ] = 0x7F & (nVal>>7);
            }
		else if( !strcmp( pszArg, "--led3" ) )
			{
            rgbData[ 0 ] = 0xAF;
			unsigned int nVal = atoi( rgpszArg[ ++iA ] );
			rgbData[ 1 ] = 0x7F & nVal; // data bits 0:6
			rgbData[ 2 ] = 0x7F & (nVal>>7);
            }
		}


    //
   	// step 1: open the OSS device for writing
    //
   	int fd = open( device, O_WRONLY, 0 );
   	if ( fd < 0 )
		{
		printf("Error: cannot open %s\n", device);
		exit(1);
		}

    //
   	// step 2: write the MIDI information to the OSS device
    //
   	int cWrite = write( fd, rgbData, sizeof(rgbData) );
	printf( "<debug cWrite='%d'/>\n", cWrite );

    //
   	// step 3: (optional) close the OSS device
    //
   	close( fd );

   	return 0;
}
// ////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////

