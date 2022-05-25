#pragma warning(disable : 4996)
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>


void mkHgram(BYTE * img, int * h,int ImgSize) {
	for (int i = 0; i < ImgSize; i++) {
		h[img[i]]++;
	}
}
void Binarization(BYTE * img, BYTE * out, int ImgSize, int Th) {
	for (int i = 0; i < ImgSize; i++) {
		if (img[i] >= Th) {
			out[i] = 255;
		}
		else {
			out[i] = 0;
		}
	}
}

double AVG(int * h, int a, int b) {
	double sum = 0.0;
	int cnt = 0;
	for (int i = a; i < b; i++) {
		sum+=(h[i] * i);
		cnt += h[i];
	}
	return (sum / (double)cnt);
}
int GonzalezBinThresh(int *h, int left, int right) {
	double m, mp,m00,m11,abbs; // m은 현재, m1은 직전.
	int low, high;
	for (int i = left; i < right; i++) {
		if (h[i]) {
			low = i;
			break;
		}
	}
	for (int i = right; i > left; i--) {
		if (h[i]) {
			high = i;
			break;
		}
	}
	m = (low + high) / 2.0;
	do{
		m00=AVG(h, low,m);
		m11 = AVG(h, m, high);
		mp = m;
		m = (m00 + m11) / 2.0;
		abbs = m - mp;
		
	} while (abbs > 3.0 || abbs<(-1.0)*3.0);
	return m;


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
	Th=Gonzalez(Histo, 0, 255);
	Binarization(Image, Output, ImgSize, Th);

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinf, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	
	free(Image); free(Output);
	fclose(fp);