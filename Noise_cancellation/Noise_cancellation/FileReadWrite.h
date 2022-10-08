#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.1415926535897932384626433832795
#define DFT_point 128
#define FrameSize 256

short* Read_file(int* length);
int File_size(void);
void Write_file(double* result, int length);
double* Windowing();
