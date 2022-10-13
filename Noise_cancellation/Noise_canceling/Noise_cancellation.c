#include "FileReadWrite.h"


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

double* Windowing()
{
	int i;
	double temp;
	double* window = malloc(sizeof(double) * FrameSize);

	for (i = 0; i < FrameSize; i++)
	{
		temp = 0.54 - 0.46 * cos(2 * pi * i / FrameSize);
		window[i] = temp / 1.08;
	}

	return window;
}


int File_size(void)
{
	FILE* fr;
	int size;

	fr = fopen("noisy.raw", "rb");

	fseek(fr, 0, SEEK_END);
	size = ftell(fr);

	fclose(fr);

	return size;
}

void Write_file(double* result, int length)
{
	FILE* fw;
	fw = fopen("output_noisy.raw", "wb");
	fwrite(result, sizeof(double), length, fw);
	fclose(fw);
}