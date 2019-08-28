#include "Func.h"

void BufCon( Buf *DB, Int OrderFlag )
{
	if( OrderFlag )
	{
		FILE *fp;
		fopen_s( &fp, FILENAME, "rb" );	

		DB->Input      = (UChar*)calloc(ROW*COL, sizeof(UChar));
		DB->CurBlk	   = (UChar*)calloc(TSIZE*TSIZE, sizeof(UChar));
		DB->DFT_Real   = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->DFT_Imag   = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->FFT_Real   = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->FFT_Imag   = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->Magnitude  = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->Phase      = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->IDFT_Real  = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->IDFT_Imag  = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->IFFT_Real  = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->IFFT_Imag  = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->Spectrum   = (Double*)calloc(ROW*COL, sizeof(Double));
		DB->Output     = (UChar*)calloc(ROW*COL, sizeof(UChar));
		DB->Filtered   = (Double*)calloc(ROW*COL, sizeof(Double));


		fread( DB->Input, sizeof(UChar), ROW*COL, fp );

		fclose(fp);
	}	
	else
	{
		free( DB->Input );
		free( DB->CurBlk );
		free( DB->DFT_Real );
		free( DB->DFT_Imag );
		free( DB->FFT_Real );
		free( DB->FFT_Imag );
		free( DB->Magnitude );
		free( DB->Phase );
		free( DB->IDFT_Real );
		free( DB->IDFT_Imag );
		free( DB->IFFT_Real );
		free( DB->IFFT_Imag );
		free( DB->Spectrum );
		free( DB->Output );		
		free( DB->Filtered );
	}
}