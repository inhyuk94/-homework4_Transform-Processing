#include "Func.h"

void DCT( Buf *DB )
{
	FILE   *fp;	
	
	fopen_s(&fp, "DCT_Spectrum.raw", "wb" );

	// DCT 식 구현
	Int    x, y, u, v, i, j;
	Int    xv, uv, ij;
	Int    Blk_Row, Blk_Col;
	Double accum, twoM, twoN, scale;
	
	twoM = 2.0 * TSIZE;
	twoN = 2.0 * TSIZE;
	scale = (2.0 / sqrt((Double)TSIZE*TSIZE));

	for (Blk_Col = 0; Blk_Col < (COL + (COL%TSIZE)) / TSIZE; Blk_Col++)
	{
		for (Blk_Row = 0; Blk_Row < (ROW + (ROW%TSIZE)) / TSIZE; Blk_Row++)
		{
			for (i = 0; i < TSIZE; i++)
			{
				for (j = 0; j < TSIZE; j++)
				{
					ij = (Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*i) + j;
					DB->CurBlk[i*TSIZE + j] = DB->Input[ij];
				}
			}

			for (v = 0; v < TSIZE; v++)
			{
				for (u = 0; u < TSIZE; u++)
				{

					accum = 0.0;

					for (y = 0; y < TSIZE; y++)
						for (x = 0; x < TSIZE; x++)
						{
							accum += cos((PI*u*(2 * x + 1)) / twoM) * cos((PI*v*(2 * y + 1)) / twoN) * DB->CurBlk[y*TSIZE + x];
						}

					accum *= scale;
					accum *= scale;

					if (u == 0) accum *= ( 1.0 / sqrt(2.0));
					if (v == 0) accum *= ( 1.0 / sqrt(2.0));
					
					uv = (Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*v) + u;
					DB->Spectrum[uv] = accum;
				}
			}
		}
	}


	if (TSIZE == ROW && TSIZE == COL) // DCT스펙트럼 셔플링 X  
	{
		Double temp;
		UChar *Dspectrum;
		Dspectrum = (UChar*)calloc(ROW*COL, sizeof(UChar));

		
		for (i = 0; i < COL; i++)
		{
			for (j = 0; j < ROW; j++)
			{
				 temp = fabs(DB->Spectrum[0]) * log10(1 + fabs(DB->Spectrum[i*ROW + j]));
				 temp = (int)(temp + 0.5);
				 Dspectrum[i*ROW + j] = temp > 255 ? 255 : temp < 0 ? 0 : temp;
			}
		}
		fwrite(Dspectrum, sizeof(UChar), ROW*COL, fp);

		free(Dspectrum);

	}




	fclose(fp);

}

void IDCT( Buf *DB )
{
	FILE   *fp;
	
	fopen_s(&fp, "DCT_ReconstructedImg.raw", "wb" );
		
	// IDCT 식 구현
	Int    u, v, x, y, i, j, temp;
	Int    xy, uv, ij;
	Int    Blk_Col,Blk_Row;
	Double accum,twoM,twoN,scale;
	Double Cu,Cv;
	Double *CurBlk;

	CurBlk = (Double*)calloc(TSIZE*TSIZE, sizeof(Double));
	twoM = 2.0* TSIZE;
	twoN = 2.0* TSIZE;
	scale = (2.0 / sqrt((Double)TSIZE*TSIZE));

	for( Blk_Col=0; Blk_Col<(COL + (COL%TSIZE)) / TSIZE; Blk_Col++ )
	{
		for (Blk_Row = 0; Blk_Row < (ROW + (ROW%TSIZE)) / TSIZE; Blk_Row++)
		{
			for (i = 0; i < TSIZE; i++)
			{
				for (j = 0; j < TSIZE; j++)
				{
					ij = (Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*i) + j;
					CurBlk[i*TSIZE + j] = DB->Spectrum[ij];
				}
			}


			for (y = 0; y < TSIZE; y++)
			{
				for (x = 0; x < TSIZE; x++)
				{

					accum = 0.0;
				
					for (v = 0; v < TSIZE; v++)
						for (u = 0; u < TSIZE; u++)
						{
							if (u == 0) Cu = 1/ sqrt(2.0);
							else Cu = 1.0;
							if (v == 0)  Cv = 1 / sqrt(2.0);
							else Cv = 1.0;
							
							accum += Cu*Cv*cos((PI*u*(2 * x + 1)) / twoM) * cos((PI*v*(2 * y + 1)) / twoN) * CurBlk[v*TSIZE + u];
						}
					
					temp = (int)(accum + 0.5);
					
					xy = (Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*y) + x;
					DB->Output[xy] = temp > 255 ? 255 : temp < 0 ? 0 : temp;;
				}
			}
		}
	}	
	
	fwrite( DB->Output, sizeof(UChar), ROW*COL, fp );

	free(CurBlk);
	fclose(fp);
}