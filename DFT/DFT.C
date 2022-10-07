#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.1415926535897932384626433832795
#define DFT_point 50

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

int File_size(void) 
{
	FILE* fr;
	int size;
	
	fr = fopen("C:\\Users\\softn\\source\\repos\\DFT\\sample1.snd", "rb");

	fseek(fr, 0, SEEK_END);
	size = ftell(fr);
	
	fclose(fr);
	
	return size;
}

void Write_file(double* result, int length)
{
	FILE* fw;
	fw = fopen("C:\\Users\\softn\\source\\repos\\DFT\\result1.snd", "wb");
	fwrite(result, sizeof(double), length, fw);
	fclose(fw);
}

int main() 
{
	int i, j, k;
	int length;
	int size;
	short* Source;
	double* real_value;
	double* image_value;
	double* out_temp;

	Source = Read_file(&length);
	size = File_size();

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

	free(real_value);
	free(image_value);
	free(out_temp);
	
}
