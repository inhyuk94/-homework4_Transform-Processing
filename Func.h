#include "InOut.h"

typedef struct _Buffer
{
	UChar  *Input;
	UChar  *CurBlk;
	Double *DFT_Real;
	Double *DFT_Imag;
	Double *FFT_Real;
	Double *FFT_Imag;
	Double *Magnitude;
	Double *Phase;
	Double *IDFT_Real;
	Double *IDFT_Imag;
	Double *IFFT_Real;
	Double *IFFT_Imag;
	Double *Spectrum;
	UChar  *Output;
	Double *Filtered;	


}Buffer;

struct Complex
{
	Double Re; 
	Double Im;
};


typedef Buffer Buf;

void SubMain( Buf *DB );

void BufCon( Buf *DB, Int OrderFlag );

void BW_LPF( Buf *DB, Int Blk_Row, Int Blk_COl, Char TranIdx );

void DFT( Buf *DB );
void IDFT( Buf *DB );

void FFT( Buf *DB );
void IFFT( Buf *DB );

void OnFFT2d( Buf *DB, Int Blk_Row , Int Blk_Col );
void OnFFT1d( struct Complex *Data, Int N, Int Log2N );
void OnIFFT2d( Buf *DB, Int Blk_Row , Int Blk_Col );
void OnIFFT1d( struct Complex *Data, Int N, Int Log2N);
void OnScramble( struct Complex *Data, Int N, Int Log2N);
void OnButterfly( struct Complex *Data, Int N, Int Log2N, Int Dir);

void DCT( Buf *DB );
void IDCT( Buf *DB );

void PSNR( Buf *DB );