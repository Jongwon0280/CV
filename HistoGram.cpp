#pragma warning(disable : 4996)
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

void mkHgram(BYTE * img, int * h,int ImgSize) {
	for (int i = 0; i < ImgSize; i++) {
		h[img[i]]++;
	}
}
void mkAHgram(BYTE * img, int * h, int *ah) {
	ah[0] = h[0];
	for (int i = 1; i < 255; i++)
	{
		ah[i] = ah[i - 1] + h[i];
	}
}

void HistogramEqualization(BYTE * img, BYTE * out, int ImgSize,int * h, int *ah) {

	double c = 255 / (double)ImgSize;
	BYTE tmp[256] = { 0 };

	for (int i = 0; i < 255; i++)
	{
		tmp[i] =(BYTE)( c * ah[i]);
	}

	for (int i = 0; i < ImgSize; i++)
	{
		out[i] = tmp[img[i]];
	}
}
void HistogramStretching(BYTE * img, BYTE * out, int ImgSize,int * h) {
	int max = 0, min = 255,dif;
	for (int i = 0; i < 255; i++) {
		if (h[i]) {
			min = i;
			break;
		}
	}
	for (int i = 255;i>0; i--) {
		if (h[i]) {
			max = i;
			break;
		}
	}
	dif = max - min;
	for (int i = 0; i < ImgSize; i++) {
		out[i] = (BYTE)(((img[i] - min) / (double)dif)* 255.0);
	}
}

void BHistogramStretching(BYTE * img, BYTE * out, int ImgSize, int * h) {
	int max = 0, min = 255;
	for (int i = 0; i < 255; i++) {
		if (h[i]) {
			min = i;
			break;
		}
	}
	for (int i = 255; i>0; i--) {
		if (h[i]) {
			max = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++) {
		if (img[i] < min) {
			out[i] = 0;
		}
		else if (img[i] > max) {
			out[i] = 255;
		}
		else {
			out[i]=(BYTE)((img[i] - min) / (double)(max - min)*255.0);
		}
	}
	
}

void main() {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinf;
	RGBQUAD hRGB[256];
	double Th = 0.0;

	FILE * fp;
	fp = fopen("LENNA.bmp", "rb");
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hinf, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	
	int ImgSize = hinf.biHeight * hinf.biWidth;
	BYTE * Image = (BYTE *)malloc(ImgSize);
	BYTE * Output = (BYTE *)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };
	mkHgram(Image, Histo, ImgSize);
	mkAHgram(Image,Histo,AHisto);
	HistogramEqualization(Image, Output, ImgSize, Histo, AHisto);
	HistogramStretching(Image, Output, ImgSize, Histo);


	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinf, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
