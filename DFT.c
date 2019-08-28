#include "Func.h"

void DFT( Buf *DB )
{
	FILE   *fp;
	Char   FilterFlag;

		
	fopen_s(&fp, "DFT_Spectrum.raw", "wb" );

	if( TSIZE == ROW && TSIZE == COL )
	{
		while( 1 )
		{
			printf("LPF를 적용 하시겠습니까?(y or n)");
			scanf_s(" %c", &FilterFlag );

			if( FilterFlag == 'y' || FilterFlag == 'n' )
			{
				system("cls");	
				break;
			}
			else
				printf("다시 입력 하세요!!\n\n");
		}
	}

	// DFT 구현

	Int    x, y, u, v, i, j;
	Int    uv;
	Int    Blk_Row, Blk_Col;


	for (Blk_Col = 0; Blk_Col < (COL + (COL%TSIZE)) / TSIZE; Blk_Col++)
	{
		for (Blk_Row = 0; Blk_Row < (ROW + (ROW%TSIZE)) / TSIZE; Blk_Row++)
		{
			for (i = 0; i < TSIZE; i++)
			{
				for (j = 0; j < TSIZE; j++)
					DB->CurBlk[i*TSIZE + j] = DB->Input[(Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*i) + j];
			}

			for (v = 0; v < TSIZE; v++)
			{
				for (u = 0; u < TSIZE; u++)
				{

					uv = (Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*v) + u;
					for (y = 0; y < TSIZE; y++)
					{


						for (x = 0; x < TSIZE; x++)
						{

							DB->DFT_Real[uv] += DB->CurBlk[y*TSIZE + x] * cos((2 * PI*(u*x + v*y)) / TSIZE);
							DB->DFT_Imag[uv] -= DB->CurBlk[y*TSIZE + x] * sin((2 * PI*(u*x + v*y)) / TSIZE);
						}
					}
					

					DB->DFT_Real[uv] *= 1.0 / (TSIZE*TSIZE);
					DB->DFT_Imag[uv] *= 1.0 / (TSIZE*TSIZE);

					DB->Magnitude[uv] = sqrt(pow(DB->DFT_Real[uv], 2.0) + pow(DB->DFT_Imag[uv], 2.0));
					DB->Phase[uv] = atan2(DB->DFT_Imag[uv], DB->DFT_Real[uv]);
					
				}
			}
			if (TSIZE == ROW && TSIZE == COL && FilterFlag == 'y')
				BW_LPF(DB, Blk_Row, Blk_Col, 'D');
			
		}
	}



	if (TSIZE == ROW && TSIZE == COL) // DFT스펙트럼  
	{
		Double temp;
		UChar *Dspectrum,*shuffling,*CurBlk;
		int k, l;

		
		Dspectrum = (UChar*)calloc(ROW*COL, sizeof(UChar));
		shuffling = (UChar*)calloc(ROW*COL, sizeof(UChar));
		CurBlk = (UChar*)calloc((ROW / 2)*(COL / 2), sizeof(UChar));

		for (i = 0; i < COL; i++)
		{
			for (j = 0; j < ROW; j++)
			{
				temp = DB->Magnitude[0] * log10(1 + fabs(DB->Magnitude[i*ROW + j]));
				temp = (int)(temp + 0.5);
				Dspectrum[i*ROW + j] = temp > 255 ? 255 : temp < 0 ? 0 : temp;

			}
		}

		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				for (k = 0; k < COL / 2; k++)
				{
					for (l = 0; l < ROW / 2; l++)
					{
						CurBlk[k*(ROW / 2) + l] = Dspectrum[(i*ROW*(COL / 2)) + j*(ROW / 2) + k*ROW + l];
					}
				}

				for (v = 0; v < COL / 2; v++)
				{
					for (u = 0; u < ROW / 2; u++)
					{
						shuffling[(i*ROW*(COL / 2)) + j*(ROW / 2) + v*ROW + u] = CurBlk[((ROW / 2)*(COL / 2) - 1) - (v*(ROW / 2) + u)];
					}
				}
			}
		}
		fwrite(shuffling, sizeof(UChar), ROW*COL, fp);

		free(Dspectrum);
		free(shuffling);
		free(CurBlk);
	}

	fclose(fp);

}

void IDFT( Buf *DB )
{
	FILE   *fp;

	fopen_s(&fp, "DFT_ReconstructedImg.raw", "wb" );

	// IDFT 구현

	Int    x, y, u, v, i, j, temp;
	Int    uv, xy,ij;
	Int    Blk_Row, Blk_Col;
	Double accum;


	for (Blk_Col = 0; Blk_Col < (COL + (COL%TSIZE)) / TSIZE; Blk_Col++)
	{
		for (Blk_Row = 0; Blk_Row < (ROW + (ROW%TSIZE)) / TSIZE; Blk_Row++)
		{
			for (i = 0; i < TSIZE; i++)
			{
				for (j = 0; j < TSIZE; j++)
				{
					ij = (Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*i) + j;
					DB->IDFT_Real[ij] = DB->Magnitude[ij] * cos(DB->Phase[ij]);
					DB->IDFT_Imag[ij] = DB->Magnitude[ij] * sin(DB->Phase[ij]);

				}
			}


			for (y = 0; y < TSIZE; y++)
			{
				for (x = 0; x < TSIZE; x++)
				{
					accum = 0.0;
					xy = (Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*y) + x;
					for (v = 0; v < TSIZE; v++)
					{
						for (u = 0; u < TSIZE; u++)
						{
							uv = (Blk_Col*(ROW + (ROW%TSIZE))*TSIZE) + (Blk_Row*TSIZE) + ((ROW + (ROW%TSIZE))*v) + u;
							accum += (DB->IDFT_Real[uv])* cos((2 * PI*(u*x + v*y)) / TSIZE) - (DB->IDFT_Imag[uv])*sin((2 * PI*(u*x + v*y)) / TSIZE);
						}
					}
					temp = (int)(accum + 0.5);
					
					DB->Output[xy] = temp>255? temp : temp < 0 ? 0 : temp;

				}
			}
		}
	}

	fwrite(DB->Output, sizeof(UChar), ROW*COL, fp);
	fclose(fp);
}
