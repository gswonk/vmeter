//
//  patMidi.cpp - $Revision: 1.49 $
//
//  rev 2013.09.08 gls Added Cylon
//  rev 2013.09.04 gls continue
//  rev 2013.09.03 gls
//


#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <inttypes.h>

#include <sys/times.h>
#include <time.h>

#include <linux/soundcard.h>

enum    Pattern
    {
    Count
    , Clock
    , Chase
    , Cylon
    , Jump
    , Rand
    , SetBit
    , Spin
    , Walk
    };

typedef unsigned char byte_t;

bool fDebug = false;

#define LedMax  38


// ////////////////////////////////////////////////////////////
void    __DumpBits()
{
#if DUMP_BITS
    for(int iB=0; iB<64; iB++)
        {
        //int64_t nMask64 = (1LL << iB);
        unsigned long long nMask = (1LL << iB);
        //printf( "<debug iB='%d' nMask='0x%llX' nMask64='0x%llX'/>\n", iB, nMask, nMask64 );
        printf( "<debug iB='%d' nMask='0x%llX'/>\n", iB, nMask );
        }
    exit( 0 );
#endif
}
// ////////////////////////////////////////////////////////////
void __WrBytes( int fd, byte_t nB0, byte_t nB1, byte_t nB2 )
{
    unsigned char rgbData[] = { 0xB0, 20, 0x31 }; // change control

    rgbData[ 0 ] = nB0;
    rgbData[ 1 ] = nB1;
    rgbData[ 2 ] = nB2;

    int cWrite = write( fd, rgbData, sizeof(rgbData) );
    if( fDebug )
        {
        printf( "<write cWr='%d' dat='0x%02X%02X%02X'/>\n", cWrite, nB0, nB1, nB2 );
        }
}
// ////////////////////////////////////////////////////////////
void    __SetBit( int fd, int iBit )
{
    iBit %= LedMax;
    unsigned long long nMask = (1LL << iBit++);
    printf( "<debug iBit='%d' nMask='0x%llX'/>\n", iBit, nMask );

    __WrBytes( fd, 0xAD, 0x7F & (nMask >> 0), 0x7F & (nMask >> 7) );
    __WrBytes( fd, 0xAE, 0x7F & (nMask >> 14), 0x7F & (nMask >> 21) );
    __WrBytes( fd, 0xAF, 0x7F & (nMask >> 28), 0x7F & (nMask >> 35) );
}
// ////////////////////////////////////////////////////////////
void    __SetBits( int fd, unsigned long long nMask )
{
    __WrBytes( fd, 0xAD, 0x7F & (nMask >> 0), 0x7F & (nMask >> 7) );
    __WrBytes( fd, 0xAE, 0x7F & (nMask >> 14), 0x7F & (nMask >> 21) );
    __WrBytes( fd, 0xAF, 0x7F & (nMask >> 28), 0x7F & (nMask >> 35) );
}
// ////////////////////////////////////////////////////////////
unsigned long long __calcBit( int iBit )
{
    unsigned long long nMask = (1LL << iBit);

    return( nMask );
}
// ////////////////////////////////////////////////////////////
int main( int cArg, char *rgpszArg[] )
{
    const char *pszDevice =  "/dev/midi1";

    unsigned char rgbData[] = { 0xB0, 20, 0x31 }; // change control
    int cPattern = -1;
    //useconds_t uDelay = 1000 * 10;  // 10 mS
    //useconds_t uDelay = 1000 * 500;  // 500 mS
    useconds_t uDelay = 1000 * 50;  // 50 mS

    Pattern patType = Count;
    int iState = 0;

    printf( "<patMidi rev='$Revision: 1.49 $'/>\n" );
    printf( "<debug sizeof(int)='%d'/>\n", INT_MAX );    // 2^32 - 1
    printf( "<debug sizeof(long)='%ld'/>\n", LONG_MAX );    // 2^32 - 1
    printf( "<debug sizeof(llong)='%lld'/>\n", LLONG_MAX ); // 2^64 - 1




    ///zzzzz
    for(int iS=0; iS<(2*LedMax); iS++)
        {
        int iB = ( iS < LedMax ?  iS : (2 * LedMax) - iS );
        printf( "<debug iS='%d' iB='%d'/>\n", iS, iB );
        }

    assert( 0 );


    for(int iA=1; iA<cArg; iA++)
        {
		char *pszArg = rgpszArg[ iA ];

		if( !strcmp( pszArg, "--db" ) )
			{
			fDebug = true;
			}
		else if( !strcmp( pszArg, "--ms" ) )
			{
			uDelay = 1000 * atoi( rgpszArg[ ++iA ] );
			}
		else if( !strcmp( pszArg, "--dev" ) )
			{
			pszDevice = rgpszArg[ ++iA ];
			}
		else if( !strcmp( pszArg, "--clock" ) )
			{
			patType = Clock;
            uDelay = 10 * 1000;  // 10 mS
			}
		else if( !strcmp( pszArg, "--count" ) )
			{
			//cPattern = atoi( rgpszArg[ ++iA ] );
			}
		else if( !strcmp( pszArg, "--chase" ) )
			{
            patType = Chase;
            }
		else if( !strcmp( pszArg, "--cylon" ) )
			{
            patType = Cylon;
            }
		else if( !strcmp( pszArg, "--jump" ) )
			{
            patType = Jump;
            }
		else if( !strcmp( pszArg, "--rnd" ) )
			{
            patType = Rand;
            }
		else if( !strcmp( pszArg, "--setbit" ) )
			{
            patType = SetBit;
			iState = atoi( rgpszArg[ ++iA ] );
            }
		else if( !strcmp( pszArg, "--spin" ) )
			{
            patType = Spin;
            }
		else if( !strcmp( pszArg, "--walk" ) )
			{
            patType = Walk;
            }
		else if( !strcmp( pszArg, "--dev1" ) )
			{
            pszDevice = "/dev/midi1";
            }
		else if( !strcmp( pszArg, "--dev2" ) )
			{
            pszDevice = "/dev/midi2";
            }
        else
            {
            assert( false );
            }
		}


    //
	// step 1: open the OSS device for writing
    //
	int fd = open( pszDevice, O_WRONLY, 0 );
	if ( fd < 0 )
		{
		printf( "<error msg='cannot open %s'/>\n", pszDevice );
		exit( 1 );
		}



    //
	// step 2: write the MIDI information to the OSS device
    //
    while( cPattern-- != 0 )
        {

        if( patType == Count )
            {
            __WrBytes( fd, 0xB0, 20, (iState++ % 128) );  // TODO!!!!
            }
        else if( patType == Chase )
            {
            iState++;
            iState %= LedMax;

            long long nMask = ( 1LL << iState );
            nMask ^= -1;        // flip the bits

            __SetBits( fd,nMask );
            //printf( "<debug.Chase nMask='0x%0llX'/>\n", nMask );
            }
        else if( patType == Clock )
            {
            clock_t nTime = times( NULL );
            __WrBytes( fd, 0xAD, 0x7F & (nTime >> 0), 0x7F & (nTime >> 7) );
            __WrBytes( fd, 0xAE, 0x7F & (nTime >> 14), 0x7F & (nTime >> 21) );
            __WrBytes( fd, 0xAF, 0x7F & (nTime >> 28), 0x00 );
            }
        else if( patType == Cylon )
            {
            int iS = iState++ % LedMax;
            unsigned long long nMask = __calcBit( iS % LedMax );
            for(int iB=0; iB<9; iB++)
                {
                nMask |= __calcBit( (iB+iS) % LedMax );
                }
            __SetBits( fd, nMask );
            }
        else if( patType == Rand )
            {
            __WrBytes( fd, 0xAD, 0x7F & random(), 0x7F & random() );
            __WrBytes( fd, 0xAE, 0x7F & random(), 0x7F & random() );
            __WrBytes( fd, 0xAF, 0x7F & random(), 0x7F & random() );
            }
        else if( patType == Spin )
            {
            __SetBit( fd, iState++ % LedMax );
            }
        else if( patType == SetBit )
            {
            __SetBit( fd, iState );
            break;
            }
        else if( patType == Walk )
            {
            __WrBytes( fd, 0xB0, 20, iState++ % (1<<7) );
            }
        else if( patType == Jump )
            {
            __WrBytes( fd, 0xB0, 20, random() % (1<<7) );
            }
        else
            {
            assert( false );
            }

        usleep( uDelay );
        }

    //
	// step 3: (optional) close the OSS device
    //
	close( fd );

	return 0;
}
// ////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////

