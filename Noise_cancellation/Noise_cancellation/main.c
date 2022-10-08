#include "FileReadWrite.h" // include <stdio.h>, <stdlib.h>, <math.h>

/*
* Read_file();
* File_size();
* Write_file();
* Windowing();
*/
int main()
{
	int i, j, k;
	int length;
	int size;
	short* Source;
	short* result_value;
	double* win_sig;
	double* IDFT_sig;
	double* real_value;
	double* image_value;
	double* out_temp;
	double difference_value;

	Source = Read_file(&length);
	size = File_size();

	result_value = malloc(sizeof(short) * size);
	win_sig = malloc(sizeof(double) * FrameSize);
	IDFT_sig = malloc(sizeof(double) * FrameSize);
	real_value = malloc(sizeof(double) * DFT_point);
	image_value = malloc(sizeof(double) * DFT_point);
	out_temp = malloc(sizeof(double) * size);

	if (Source == NULL)
	{
		printf("Read error");
		return 0;
	}

	printf("File Size : %d Byte", size);

	// initialization
	for (i = 0; i < size; i++)
	{
		out_temp[i] = 0;
	}

	// DFT
	for (i = 0; i < DFT_point; i++)
	{
		real_value[i] = image_value[i] = 0;
	}

	for (j = 0; j < DFT_point; j++)
	{
		for (k = 0; k < DFT_point; k++)
		{
			real_value[j] += Source[k] * cos(2 * pi * j * k / DFT_point);
			image_value[j] -= Source[k] * sin(2 * pi * j * k / DFT_point);
		}
	}


	Write_file(real_value, DFT_point);

	free(result_value);
	free(win_sig);
	free(IDFT_sig);
	free(real_value);
	free(image_value);
	free(out_temp);
}