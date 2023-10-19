#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <Windows.h>
#include <iostream>

using namespace std;

cv::Mat src;


BITMAPINFOHEADER createBitmapHeader(int width, int height) {

	//specify image format by using bitmapinfoheader
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0; //because no compression
	bi.biXPelsPerMeter = 0; //we
	bi.biYPelsPerMeter = 0; //we
	bi.biClrUsed = 0; //we ^^
	bi.biClrImportant = 0; //still we

	return bi;
};

cv::Mat hwnd2mat(HWND hWnd)
{
    cv::Mat mat;

	HDC deviceContext = GetDC(hWnd);
	HDC memoryDeviceContext = CreateCompatibleDC(deviceContext);
	SetStretchBltMode(memoryDeviceContext, COLORONCOLOR);

	RECT windowRect;
	GetClientRect(hWnd, &windowRect);	//get client coordinates relative to itself excluding title bar

	int height = windowRect.bottom;
	int width = windowRect.right;

	mat.create(height, width, CV_8UC4); // 8 bit unsigned ints 4 Channels -> RGBA

	HBITMAP bitmap = CreateCompatibleBitmap(deviceContext, width, height);
	BITMAPINFOHEADER bi = createBitmapHeader(width, height);

	SelectObject(memoryDeviceContext, bitmap);

	//copy window image into bitmap
	StretchBlt(memoryDeviceContext, 0, 0, width, height, deviceContext, 0, 0, width, height, SRCCOPY);
	//transform data & store into mat.data
	GetDIBits(memoryDeviceContext, bitmap, 0, height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	//clean up
	DeleteObject(bitmap);
	DeleteDC(memoryDeviceContext);
	ReleaseDC(hWnd, deviceContext);

	return mat;
}



int main(int argc, char** argv)
{
	int key = 0;
	HWND hwndDesktop = FindWindow(NULL, L"Tower Unite  ");
	cv::namedWindow("output", cv::WINDOW_NORMAL);
	cv::resizeWindow("output", 1280, 720);

    while (key != 27)
    {
		src = hwnd2mat(hwndDesktop);
        // you can do some image processing here
        cv::imshow("output", src);
        key = cv::waitKey(1); // you can change wait time
    }
	cv::destroyWindow("output");
}