#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.1415926535897932384626433832795
#define Frame_Size 100
#define DFT_point 100

short* Read_file(int* length)
{
	short* Source;
	FILE* fr;

	fr = fopen("C:\\Users\\softn\\source\\repos\\DFT\\sample1.snd", "rb");

	fseek(fr, 0, SEEK_END);
	*length = ftell(fr) / sizeof(short);
	fseek(fr, 0, SEEK_SET);
	Source = malloc(sizeof(short) * (*length));
	fread(Source, sizeof(short), *length, fr);

	fclose(fr);

	return Source;
}

double* Genarate_HW()
{
	int i;
	double temp;
	double* window = (double*)malloc(sizeof(double) * Frame_Size);
	//////////////////Hamming Window 생성//////////
	for (i = 0;i < Frame_Size;i++)
	{
		temp = 0.54 - 0.46 * cos(2 * pi * i / Frame_Size);
		window[i] = temp / 1.08;
	}
	return window;
}

void Write_file1(double* result, int length)
{
	FILE* fw;
	fw = fopen("C:\\Users\\softn\\source\\repos\\DFT\\result1.snd", "wb");
	fwrite(result, sizeof(double), length, fw);
	fclose(fw);
}

void Write_file2(short* result, int length)
{
	FILE* fw;
	fw = fopen("C:\\Users\\softn\\source\\repos\\DFT\\result2.snd", "wb");
	fwrite(result, sizeof(short), length, fw);
	fclose(fw);
}

void main()
{
	int i, j, k;
	short* Source;
	short* result_value;
	double* real_value;
	double* image_value;
	double* Hamming_window;
	double* IDFT_sig;
	double* out_temp;
	double* win_sig;
	int length;
	double max_freq = 0;
	double max;
	double difference = 0;

	Source = Read_file(&length); // Sampling rate = 16,000Hz
	Hamming_window = Genarate_HW();

	result_value = (short*)malloc(sizeof(short) * length);
	real_value = (double*)malloc(sizeof(double) * Frame_Size);
	image_value = (double*)malloc(sizeof(double) * Frame_Size);
	IDFT_sig = (double*)malloc(sizeof(double) * Frame_Size);
	out_temp = (double*)malloc(sizeof(double) * length);
	win_sig = (double*)malloc(sizeof(double) * Frame_Size);

	if (Source == NULL)
	{
		printf("Read error!\n");
		return;
	}

	printf("input file size : %zd\n", length * sizeof(short));

	for (i = 0; i < length; i++)
		out_temp[i] = 0;


	for (i = 0;i < length;i += Frame_Size / 2)
	{
		// windowing //
		for (j = 0;j < Frame_Size;j++)
			if (i + j < length)
				win_sig[j] = Source[i + j] * Hamming_window[j];
			else	// 마지막 Frame에서 source의 length를 초과할 경우에
				win_sig[j] = 0;
		if (i == 0)	// 첫번째 Frame일 경우
			for (j = 0;j < Frame_Size / 2;j++)
				win_sig[j] += Source[i + j] * Hamming_window[j + Frame_Size / 2];

		max = 0;
		max_freq = 0;

		// DFT //
		for (j = 0;j < DFT_point;j++)
			real_value[j] = image_value[j] = 0;

		for (j = 0;j < DFT_point;j++)
		{
			for (k = 0;k < DFT_point;k++)
			{
				real_value[j] += win_sig[k] * cos(2 * pi * k * j / DFT_point);

				if (j == 0)
					max = real_value[0];
				else
					if (max < real_value[j])
					{
						max = real_value[j];
						max_freq = k * j / DFT_point;
					}

				image_value[j] -= win_sig[k] * sin(2 * pi * k * j / DFT_point);
			}
		}

		printf("%d번째 window에서 max값과 그 때의 주파수 %lf, %lf\n", (i / Frame_Size) + 1, max, max_freq * (double)(length / DFT_point));



		// IDFT //
		for (j = 0; j < DFT_point; j++)
			IDFT_sig[j] = 0;

		for (j = 0; j < DFT_point; j++)
		{
			for (k = 0; k < DFT_point; k++)
			{
				IDFT_sig[j] += real_value[k] * cos(2 * pi * j * k / DFT_point) / DFT_point;
				IDFT_sig[j] -= image_value[k] * sin(2 * pi * j * k / DFT_point) / DFT_point;
			}
		}

		for (j = 0;j < Frame_Size;j++)
			if (i + j < length)
				out_temp[i + j] += IDFT_sig[j];
	}
		
	for (i = 0;i < length;i++)
		result_value[i] = (short)(floor(out_temp[i] + 0.5));

	for (i = 0;i < length;i++)
	{
		difference += pow((double)result_value[i] - (double)Source[i],2);
		printf("%d : %d\n" , result_value[i], Source[i]);
	}

	printf("원본과 결과 값의 차이 : %.f\n", difference);

	Write_file2(result_value, length);	

	free(Source);
	free(result_value);
	free(real_value);
	free(image_value);
	free(IDFT_sig);
	free(out_temp);
	free(Hamming_window);
	free(win_sig);
	
}