#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
void Swap(BYTE * a, BYTE * b) { // 스왑함수
	BYTE tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;

}
BYTE Median(BYTE * tmp,int size) {
	for (int i = 0; i < size-1; i++) { // 버블정렬
		for (int j = i + 1; j <= size - 1; j++) {
			if (tmp[j-1] > tmp[j]) {
				Swap(tmp +(j-1), tmp + j);
			}
		}
	}
	return tmp[size / 2]; // 중간값에 위치하는 값 반환..
}
void InverseImage(BYTE* Img, BYTE *Out, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + Val > 255)
		{
			Out[i] = 255;
		}
		else if (Img[i] + Val < 0)
		{
			Out[i] = 0;
		}
		else 	Out[i] = Img[i] + Val;
	}
}
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] * Val > 255.0)
		{
			Out[i] = 255;
		}
		else 	Out[i] = (BYTE)(Img[i] * Val);
	}
}

void ObtainHistogram(BYTE* Img, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++;
	}
}

void ObtainAHistogram(int* Histo, int* AHisto)
{
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			AHisto[i] += Histo[j];
		}
	}
}

void HistogramStretching(BYTE * Img, BYTE * Out, int * Histo, int W, int H)
{
	int ImgSize = W * H;
	BYTE Low, High;
	for (int i = 0; i < 256; i++) {
		if (Histo[i] != 0) {
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			High = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0);
	}
}
void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H)
{
	int ImgSize = W * H;
	int Nt = W * H, Gmax = 255;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256];
	for (int i = 0; i < 256; i++) {
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = NormSum[Img[i]];
	}
}

void Binarization(BYTE * Img, BYTE * Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}



void AverageConv(BYTE* Img, BYTE* Out, int W, int H) // 박스평활화
{
	double Kernel[3][3] = { 0.11111, 0.11111, 0.11111,
		0.11111, 0.11111, 0.11111,
		0.11111, 0.11111, 0.11111 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m)*W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H) // 가우시안평활화
{
	double Kernel[3][3] = { 0.0625, 0.125, 0.0625,
		0.125, 0.25, 0.125,
		0.0625, 0.125, 0.0625 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
		-1.0, 0.0, 1.0,
		-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}
void Median_Filterling(BYTE * Image, BYTE * Output,BITMAPINFOHEADER * hInfo,int size) {
	int Length = size;  // 마스크의 한 변의 길이

	int Margin = Length / 2; // 가장자리 처리를 위한 여백

	int WSize = Length * Length; // 마스크 사이즈

	BYTE* temp = (BYTE*)malloc(sizeof(BYTE) * WSize); // 동적으로 사이즈 받아오기

	int W = hInfo->biWidth, H = hInfo->biHeight; // 비트맵 파일 너비 높이

	int i, j, m, n; // 반복문 변수

	for (i = Margin; i < H - Margin; i++) { // 가장자리를 제외한 Image에서 행

		for (j = Margin; j < W - Margin; j++) { // 가장자지를 제외한 Image에서 열

			for (m = -Margin; m <= Margin; m++) { // 마스크 행 위치

				for (n = -Margin; n <= Margin; n++) { // 마스크 열 위치

					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m)*W + j + n];
					// (m + margin/ n + margin)은 음수값이 안나오게 하기 위함 . 1차원배열에서 처리하기위해 너비값을 곱해줌. 동적으로 받아온 temp배열에 담아줌.
					// Image에서 i,j에 m,n을 더해줌으로써, 각 위치에 있는 화소정보를 Temp로 받아서 저장해줌.  

				}

			}

			Output[i * W + j] = Median(temp, WSize); // temp에서 중간값을 구해주기위해, temp배열과 WSize를 넘겨줌으로써, 중간에 위치한 화소값 구해준뒤. Output의 i,j의 위치에 넣어줌.

		}

	}

	free(temp);
}
void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
		0.0, 0.0, 0.0,
		1.0, 1.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
		-2.0, 0.0, 2.0,
		-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -2.0, -1.0,
		0.0, 0.0, 0.0,
		1.0, 2.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
		-1.0, 8.0, -1.0,
		-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
		-1.0, 9.0, -1.0,
		-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			//Out[i * W + j] = abs((long)SumProduct) / 8;
			if (SumProduct > 255.0) Out[i * W + j] = 255;
			else if (SumProduct < 0.0) Out[i * W + j] = 0;
			else Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

int main()
{
	
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트
	FILE* fp;
	fp = fopen("lenna.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE * Image = (BYTE *)malloc(ImgSize);
	BYTE * Temp = (BYTE *)malloc(ImgSize);
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };


	//AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//Median_Filterling(Image, Output, &hInfo,3);
	
	//Laplace_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);

	//GaussAvrConv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	//Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	//Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight); // 계수합이 1이면 유지, 1보다작으면 어두워 1보다크면 밝

	//Prewitt_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	//Prewitt_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	
	

	fp = fopen("c1.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	free(Image);
	free(Output);
	
	return 0;
}