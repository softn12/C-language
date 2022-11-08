#include "swear_word_filter.h"

short* Read_file(int* length)
{
	short* Source;
	FILE* fr;

	/* "sample.snd"
	* size : 2,294KB 
	* sampling rate : 44,100Hz
	* 16bit signed mono 
	*/
	fr = fopen("sample.snd", "rb");

	fseek(fr, 0, SEEK_END);
	*length = ftell(fr) / sizeof(short); // length == file size
	fseek(fr, 0, SEEK_SET);
	Source = malloc(sizeof(short) * (*length));
	fread(Source, sizeof(short), *length, fr);

	fclose(fr);

	return Source;
}

short* SS1(int* ss1_length)
{
	short* Source;
	FILE* fr;
	/* "ssibal.snd"
	* size : 49KB
	* sampling rate : 44,100Hz
	* 16bit signed mono
	*/
	fr = fopen("ssibal1.snd", "rb");

	fseek(fr, 0, SEEK_END);
	*ss1_length = ftell(fr) / sizeof(short);
	fseek(fr, 0, SEEK_SET);
	Source = malloc(sizeof(short) * (*ss1_length));
	fread(Source, sizeof(short), *ss1_length, fr);

	fclose(fr);

	return Source;
}

short* SS2(int* ss2_length)
{
	short* Source;
	FILE* fr;
	/* "ssibal.snd"
	* size : 60KB
	* sampling rate : 44,100Hz
	* 16bit signed mono
	*/
	fr = fopen("ssibal2.snd", "rb");

	fseek(fr, 0, SEEK_END);
	*ss2_length = ftell(fr) / sizeof(short);
	fseek(fr, 0, SEEK_SET);
	Source = malloc(sizeof(short) * (*ss2_length));
	fread(Source, sizeof(short), *ss2_length, fr);

	fclose(fr);

	return Source;
}


short* SS3(int* ss3_length)
{
	short* Source;
	FILE* fr;
	/* "ssibal.snd"
	* size : 42KB
	* sampling rate : 44,100Hz
	* 16bit signed mono
	*/
	fr = fopen("ssibal3.snd", "rb");

	fseek(fr, 0, SEEK_END);
	*ss3_length = ftell(fr) / sizeof(short);
	fseek(fr, 0, SEEK_SET);
	Source = malloc(sizeof(short) * (*ss3_length));
	fread(Source, sizeof(short), *ss3_length, fr);

	fclose(fr);

	return Source;
}

short* SS4(int* ss4_length)
{
	short* Source;
	FILE* fr;
	/* "ssibal.snd"
	* size : 67KB
	* sampling rate : 44,100Hz
	* 16bit signed mono
	*/
	fr = fopen("ssibal4.snd", "rb");

	fseek(fr, 0, SEEK_END);
	*ss4_length = ftell(fr) / sizeof(short);
	fseek(fr, 0, SEEK_SET);
	Source = malloc(sizeof(short) * (*ss4_length));
	fread(Source, sizeof(short), *ss4_length, fr);

	fclose(fr);

	return Source;
}


void Write_file(short* result, int length)
{
	FILE* fw;
	fw = fopen("result.raw", "wb");
	fwrite(result, sizeof(short), length, fw);
	fclose(fw);
}

