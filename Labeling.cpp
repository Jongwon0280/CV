#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

void Binarization(BYTE * Img, BYTE * Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}
int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}


// GlassFire 알고리즘을 이용한 라벨링 함수
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;

	// 스택으로 사용할 메모리 할당
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // 메모리 초기화

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//관심 픽셀이 영상경계를 벗어나면 처리 안함
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // 현재 라벨로 마크
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 255.0f / (float)curColor; // 0-255에서 25개영역으로 나누어서 표현.

	// 가장 면적이 넓은 영역을 찾아내기 위함 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i; //컴포넌트별 사이즈 크기  BlobArea
	}
	// CutImage 배열 클리어~ // 이진영상
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	// coloring에 저장된 라벨링 결과중 (Out_Area에 저장된) 영역이 가장 큰 것만 CutImage에 저장
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // 가장 큰 것만 저장 (size filtering)
													   //if (BlobArea[coloring[k]] > 500) CutImage[k] = 0;  // 특정 면적이상되는 영역만 출력
													//   CutImage[k] = (unsigned char)(coloring[k] * grayGap);
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}

void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y)
{
	for (int i = LU_X; i < RD_X; i++) 	Img[LU_Y * W + i] = 255;
	for (int i = LU_X; i < RD_X; i++) 	Img[RD_Y * W + i] = 255;
	for (int i = LU_Y; i < RD_Y; i++) 	Img[i * W + LU_X] = 255;
	for (int i = LU_Y; i < RD_Y; i++) 	Img[i * W + RD_X] = 255;
}

// Img: 가로/세로 라인을 그릴 이미지배열, W: 영상 가로사이즈, H: 영상 세로사이즈,
// Cx: 가로/세로 라인이 교차되는 지점의 X좌표
// Cy: 가로/세로 라인이 교차되는 지점의 Y좌표
void DrawCrossLine(BYTE* Img, int W, int H, int Cx, int Cy)
{
	for (int i = 0; i < W - 1; i++) // horizontal line
		Img[Cy * W + i] = 255;
	for (int i = 0; i < H - 1; i++) // vertical line
		Img[i * W + Cx] = 255;
}
// 컴포넌트 라벨링(Labelling)
// 이진화(포그라운드(조건에 부합하는 영역) / 백그라운드) -> 컴포넌트 라벨링 (서로 덩어리 영역 각각 레이블부여)
// 원형도
void Obtain2DCenter(BYTE* Image, int W, int H, int* Cx, int* Cy)
{
	int SumX = 0, SumY = 0;
	int cnt = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Image[i * W + j] == 0) // 동공영역이면
			{
				// (0좌표값 /  counting  )Avg
				SumY += i;
				SumX += j;
				cnt++;
			}
		}
	}
	if (cnt == 0) 	cnt = 1;
	*Cx = SumX / cnt;
	*Cy = SumY / cnt;
	//printf("%d    %d\n", Cx, Cy);
}

void Obtain2DBoundingBox(BYTE* Image, int W, int H, int* LUX, int* LUY, int* RDX, int* RDY)
{
	int flag = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Image[i * W + j] == 0) {
				*LUY = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
	flag = 0;
	for (int i = H - 1; i >= 0; i--) {
		for (int j = 0; j < W; j++) {
			if (Image[i * W + j] == 0) {
				*RDY = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
	flag = 0;

	for (int j = 0; j < W; j++) {
		for (int i = 0; i < H; i++) {
			if (Image[i * W + j] == 0) {
				*LUX = j;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
	flag = 0;
	for (int j = W - 1; j >= 0; j--) {
		for (int i = 0; i < H; i++) {
			if (Image[i * W + j] == 0) {
				*RDX = j;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
}
int main()
{
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트
	FILE* fp;
	fp = fopen("LENNA.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE * Image = (BYTE *)malloc(ImgSize);
	BYTE * Temp = (BYTE*)malloc(ImgSize); // 임시배열
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	int H = hInfo.biHeight, W = hInfo.biWidth;
	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };
	Binarization(Image,Temp, W, H,40 );
	InverseImage(Temp, Temp, W, H);
	m_BlobColoring(Temp, H, W);
	int Cx, Cy;
	int LUX, LUY, RDX, RDY;

	Obtain2DCenter(Output, W, H, &Cx, &Cy); // 이진영상의 무게중심 구하기
	Obtain2DBoundingBox(Output, W, H, &LUX, &LUY, &RDX, &RDY); // 이진영상의 외접직사각형 좌표 추출
	DrawCrossLine(Image, W, H, Cx, Cy);
	DrawRectOutline(Image, W, H, LUX, LUY, RDX, RDY);
	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "output.bmp");

	free(Image);
	free(Output);
	free(Temp);
	return 0;
}

