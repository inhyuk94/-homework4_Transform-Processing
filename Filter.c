#include "Func.h"

#define D0				32.0 // Cut off frequency (����)
#define N0				4.0  // Filter dimension (����)

void BW_LPF(Buf *DB, Int Blk_Row, Int Blk_COl, Char TranIdx)
{
	// ���Ϳ��� LPF ���� 
	// TranIdx �� ��ȯ ������ �ǹ�(DFT�� FFT��)

	int x, y;
	int tempx, tempy;
	int halfcols, halfrows;
	double butterworth, coordinate;
	halfcols = COL / 2;
	halfrows = ROW / 2;

	for (y = 0; y < COL; y++)
	{
		if (y >= halfcols)
			tempy = y - halfcols;
		else
			tempy = y + halfcols;

		for (x = 0; x < ROW; x++)
		{

			if (x >= halfrows)
				tempx = x - halfrows;
			else
				tempx = x + halfrows;

			coordinate = sqrt(pow((double)(tempx - halfcols), 2.0) + pow((double)(tempy - halfrows), 2.0));
			butterworth = 1.0 / (1.0 + pow(coordinate / D0, 2 * N0));
			if (TranIdx == 'D')
			{
				DB->DFT_Real[y*ROW + x] *= butterworth;
				DB->DFT_Imag[y*ROW + x] *= butterworth;
			}
			else if (TranIdx == 'F')
			{
				DB->FFT_Real[y*ROW + x] *= butterworth;
				DB->FFT_Imag[y*ROW + x] *= butterworth;
			}
		}

	}
}

	