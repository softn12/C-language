#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.1415926535897932384626433832795
#define Frame_Size 256
#define DFT_point 256

short* Read_file(int* length)
{
	short* Source;
	FILE* fr;

	fr = fopen("noisy.raw", "rb");

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
	fw = fopen("DFT.raw", "wb");
	fwrite(result, sizeof(double), length, fw);
	fclose(fw);
}

void Write_file2(short* result, int length)
{
	FILE* fw;
	fw = fopen("result.raw", "wb");
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
	double* DFT_abs;
	double* denoising;
	double* win_sig;
	double* phase;
	double* noise_avr;
	int length;
	double difference = 0;
	int cnt = 0;

	Source = Read_file(&length); // Sampling rate = 16,000Hz
	Hamming_window = Genarate_HW();

	result_value = (short*)malloc(sizeof(short) * length);
	real_value = (double*)malloc(sizeof(double) * Frame_Size);
	image_value = (double*)malloc(sizeof(double) * Frame_Size);
	IDFT_sig = (double*)malloc(sizeof(double) * Frame_Size);
	out_temp = (double*)malloc(sizeof(double) * length);
	win_sig = (double*)malloc(sizeof(double) * Frame_Size);
	DFT_abs = (double*)malloc(sizeof(double) * Frame_Size);
	denoising = (double*)malloc(sizeof(double) * Frame_Size);
	phase = (double*)malloc(sizeof(double) * Frame_Size);
	noise_avr = (double*)malloc(sizeof(double) * Frame_Size);

	if (Source == NULL)
	{
		printf("Read error!\n");
		return;
	}

	printf("input file length : %d\n", length);

	// initailize //
	//==========================================================
	for (i = 0; i < length; i++)
		out_temp[i] = 0;

	for (i = 0; i < Frame_Size; i++)
	{
		DFT_abs[i] = 0;
		phase[i] = 0;
		denoising[i] = 0;
		noise_avr[i] = 0;
	}
	

	// Noise Signal Spectrun Acquisition
	// 5-Frame from first frame
	for (i = 0;i < Frame_Size * 5;i += Frame_Size/2)
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

		// Noise Signal DFT //
		for (j = 0;j < DFT_point;j++)
			real_value[j] = image_value[j] = 0;

		for (j = 0;j < DFT_point;j++)
		{
			for (k = 0;k < DFT_point;k++)
			{
				real_value[j] += win_sig[k] * cos(2 * pi * k * j / DFT_point);
				image_value[j] -= win_sig[k] * sin(2 * pi * k * j / DFT_point);
			}
		}

		for (j = 0; j < DFT_point; j++)
			DFT_abs[j] = 0;

		// Absolute value of DFT signal
		for (j = 0; j < DFT_point; j++)
			DFT_abs[j] = sqrt(pow(real_value[j], 2) + pow(image_value[j], 2));

		for (j = 0; j < DFT_point; j++)
			noise_avr[j] += (DFT_abs[j] / 8);
	}


	for (i = 0;i < length;i += Frame_Size/2)
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

		// DFT //
		for (j = 0;j < DFT_point;j++)
			real_value[j] = image_value[j] = 0;

		for (j = 0;j < DFT_point;j++)
		{
			for (k = 0;k < DFT_point;k++)
			{
				real_value[j] += win_sig[k] * cos(2 * pi * k * j / DFT_point);
				image_value[j] -= win_sig[k] * sin(2 * pi * k * j / DFT_point);
			}
		}

		for (j = 0; j < DFT_point; j++)
			DFT_abs[j] = 0;

		for (j = 0; j < DFT_point; j++)
		{
			DFT_abs[j] = sqrt(pow(real_value[j], 2) + pow(image_value[j], 2));
			phase[j] = atan2(image_value[j], real_value[j]);
		}

		for (j = 0; j < DFT_point; j++)
		{
			DFT_abs[j] -= noise_avr[j];
		}
		
		
		for (j = 0; j < DFT_point; j++)
		{
			denoising[j] = DFT_abs[j] * cos(phase[j]);
			denoising[j] = DFT_abs[j] * sin(phase[j]);
		} 
		

		// IDFT //
		for (j = 0; j < DFT_point; j++)
			IDFT_sig[j] = 0;

		for (j = 0; j < DFT_point; j++)
		{
			for (k = 0; k < DFT_point; k++)
			{
				IDFT_sig[j] += denoising[k] * cos(2 * pi * j * k / DFT_point) / DFT_point;
				IDFT_sig[j] += denoising[k] * sin(2 * pi * j * k / DFT_point) / DFT_point;
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
		difference += sqrt(pow((double)result_value[i] - (double)Source[i], 2));
		printf("%d : %d\n", result_value[i], Source[i]);
	}

	//Write_file1(DFT_abs, DFT_point);

	printf("원본과 결과 값의 차이 : %lf\n", difference);

	Write_file2(result_value, length);

	free(Source);
	free(Hamming_window);
	free(result_value);
	free(real_value);
	free(image_value);
	free(IDFT_sig);
	free(denoising);
	free(out_temp);
	free(win_sig);
	free(DFT_abs);
	free(phase);
	free(noise_avr);
}