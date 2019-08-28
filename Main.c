#include "Func.h"

void main()
{		
	Buf InOut;	

	BufCon( &InOut, TRUE );
	
	SubMain( &InOut );

	BufCon( &InOut, FALSE );
}

void SubMain( Buf *DB )
{
	Int TrIdx;

	while( 1 )
	{
		printf("1. DFT\n2. FFT\n3. DCT\n\n");
		printf("���� �� ��ȯ�� �����Ͻÿ� : ");
		scanf_s(" %d", &TrIdx );

		if( TrIdx == 1 || TrIdx == 2 || TrIdx == 3)
		{
			system("cls");	
			break;
		}
		else
			printf("\n�ٽ� �Է� �ϼ���!!\n");
	}

	if( TrIdx == 1 )
	{		
		DFT( DB );
		IDFT( DB );
	}
	else if( TrIdx == 2)
	{
		FFT( DB );
		IFFT( DB );
	}
	else
	{
		DCT( DB );
		IDCT( DB );
	}	

	PSNR( DB );
}