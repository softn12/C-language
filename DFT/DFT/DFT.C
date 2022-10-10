#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.1415926535897932384626433832795

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

void Write_file(double* result, int length)
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
	double* IDFT_sig;
	int length;
	double difference = 0;

	Source = Read_file(&length);

	result_value = (short*)malloc(sizeof(short) * length);
	real_value = (double*)malloc(sizeof(double) * length);
	image_value = (double*)malloc(sizeof(double) * length);
	IDFT_sig = (double*)malloc(sizeof(double) * length);

	if (Source == NULL)
	{
		printf("Read error!\n");
		return;
	}


	// DFT //
	// 입력받은 신호 "sample1.snd"를
	// N개(length개)의 유한시간 신호라고 생각하고 DFT

	for (j = 0;j < length;j++)
		real_value[j] = image_value[j] = 0;

	for (j = 0;j < length;j++)
	{
		for (k = 0;k < length;k++)
		{
			real_value[j] += Source[k] * cos(2 * pi * k * j / length);
			image_value[j] -= Source[k] * sin(2 * pi * k * j / length);
		}
	}

	printf("input file size : %zd\n", length * sizeof(short));

	Write_file(real_value, length);
	
	// IDFT //

	for (i = 0; i < length; i++)
		IDFT_sig[i] = 0;

	for (j = 0; j < length; j++)
	{
		for (k = 0; k < length; k++)
		{
			IDFT_sig[j] += real_value[k] * cos(2 * pi * j * k / length) / length;
			IDFT_sig[j] -= image_value[k] * sin(2 * pi * j * k / length) / length;
		}

	}
	
	for (i = 0; i < length; i++)
		result_value[i] = 0;

	for (i = 0; i < length; i++)
		result_value[i] = (short)(floor(IDFT_sig[i] + 0.5));

	for (i = 0;i < length;i++)
	{
		difference+= pow((double)result_value[i] - (double)Source[i], 2);
		printf("%hd : %hd\n" , result_value[i], Source[i]);
	}

	printf("원본과 결과의 차이값 : %d\n", difference);

	Write_file2(result_value, length);

	free(Source);
	free(result_value);
	free(real_value);
	free(image_value);
	free(IDFT_sig);
}