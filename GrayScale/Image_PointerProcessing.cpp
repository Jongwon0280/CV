#pragma warning(disable : 4996)
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

void InverseImage(BYTE * img, BYTE * out, int ImgSize) {
	for (int i = 0; i < ImgSize; i++) {
		out[i] = 255 - img[i];
	}
}

void Brightness(BYTE * img, BYTE * out, int ImgSize,int val) {
	for (int i = 0; i < ImgSize; i++) {
		if (img[i] + val > 255) {
			out[i] = 255;
		}
		else if (img[i] + val < 0) {
			out[i] = 0;
		}
		else {
			out[i] = img[i] + val;
		}
	}
}

void ContrastAdj(BYTE * img, BYTE * out, int ImgSize, double val) {
	for (int i = 0; i < ImgSize; i++)
	{
		if (img[i] * val > 255.0) {
			out[i] = 255;
		}
		else {
			out[i] = (BYTE)(img[i] * val);
		}

	}
}

void main() {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinf;
	RGBQUAD hRGB[256];

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

	InverseImage(Image, Output, ImgSize); Brightness(Image, Output, ImgSize, 50);
	Brightness(Image, Output, ImgSize, (-1)*70);
	
	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinf, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	
	free(Image); free(Output);
	fclose(fp);
