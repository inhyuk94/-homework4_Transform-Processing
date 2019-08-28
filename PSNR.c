#include "Func.h"

void PSNR( Buf *DB )
{
	Int i, j;
	Double mse=0, psnr=0, max=0;
	UChar load_1=0, load_2=0;	

	for( i=0; i<COL; i++ )
	{	
		for( j=0; j<ROW; j++ )
		{
			load_1 = DB->Input[i * COL + j];
			load_2 = DB->Output[i * COL + j];

			mse += ((load_1 - load_2) * (load_1 - load_2));
			if(load_1 > max)
				max = load_1;
		}
	}

	mse = mse / (ROW*COL);
	printf("MSE : %lf\n", mse);
	psnr = 20 * log10(max / sqrt(mse));
	printf("PSNR : %lf\n", psnr);

	if( mse == 0 )
		printf("\n원본영상과 복원영상 일치♥\n\n");
	else
		printf("\n원본영상과 복원영상 불일치ㅗ\n\n");
} 