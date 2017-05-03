#pragma once
//#include"data.h"
#include<string>
#include<stdio.h>
#include<iostream>

using namespace std;

typedef unsigned short int  _WORD;
typedef unsigned       int  _DWORD;
typedef                int  _LONG;
typedef unsigned      char  _UCHAR;

typedef struct {
	_WORD     bfType = 0;         // 0x4d42 | 0x4349 | 0x5450
	_DWORD    bfSize = 0;         // размер файла
	_WORD     bfReserved1 = 0;     // 0
	_WORD     bfReserved2 = 0;     // 0
	_DWORD    bfOffBits = 0;      // смещение до поля данных,
								 // обычно 54 = 16 + biSize

	_DWORD    biSize = 0;         // размер струкуры в байтах:
								 // 40(BITMAPINFOHEADER) или 108(BITMAPV4HEADER)
								 // или 124(BITMAPV5HEADER)
	_LONG    biWidth = 0;         // ширина в точках
	_LONG    biHeight = 0;        // высота в точках
	_WORD    biPlanes = 1;        // всегда должно быть 1
	_WORD    biBitCount = 24;      // 0 | 1 | 4 | 8 | 16 | 24 | 32
	_DWORD   biCompression = 0;   // BI_RGB | BI_RLE8 | BI_RLE4 |
						 // BI_BITFIELDS | BI_JPEG | BI_PNG
								 // реально используется лишь BI_RGB
	_DWORD   biSizeImage = 0;     // Количество байт в поле данных
							 // Обычно устанавливается в 0
	_LONG    biXPelsPerMeter = 0; // горизонтальное разрешение, точек на дюйм
	_LONG    biYPelsPerMeter = 0; // вертикальное разрешение, точек на дюйм
	_DWORD   biClrUsed = 0;       // Количество используемых цветов
							 // (если есть таблица цветов)
	_DWORD   biClrImportant = 0;  // Количество существенных цветов.
								 // Можно считать, просто 0
}     BMPheader;

enum { COLOR, GRAY, COMPACT_GRAY };

class Bitmap {

private:
	_DWORD READDWORD(_UCHAR * &buf) {
		_DWORD tmp = *(_DWORD*)buf;
		buf = buf + sizeof(_DWORD);
		return tmp;
	}

	_WORD READWORD(_UCHAR * &buf) {
		_WORD tmp = *(_WORD*)buf;
		buf = buf + sizeof(_WORD);
		return tmp;
	}

	_LONG READLONG(_UCHAR * &buf) {
		_LONG tmp = *(_LONG*)buf;
		buf = buf + sizeof(_LONG);
		return tmp;
	}


public:

	BMPheader bmphead;
	//! in pixel
	int width;

	int height, byteCount;
	_UCHAR * data;

	Bitmap() {
		data = 0;
		width = height = 0;
		byteCount = 3;
	}

	Bitmap(string image_name, int pixel_format = COLOR) {
		data = 0;
		width = height = 0;
		byteCount = 3;

		switch (pixel_format)
		{
		case 0: {
			read_bmp(image_name);
			break;
		}
		case 1: {
			read_bmp(image_name);
			grayscale();
			break;
		}
		case 2: {
			read_bmp(image_name);
			compact_grayscale();
			break;
		}
		default: {
			read_bmp(image_name);
			break;}
		}

	};

	Bitmap(Bitmap & tmp) {
		bmphead = tmp.bmphead;
		width = tmp.width;
		height = tmp.height;
		byteCount = tmp.byteCount;
		if (data != 0) delete[] data;
		data = new _UCHAR[width * height * byteCount];
		memcpy(data, tmp.data, width * height * byteCount);
	}

	Bitmap & operator=(Bitmap & tmp) {
		bmphead = tmp.bmphead;
		width = tmp.width;
		height = tmp.height;
		byteCount = tmp.byteCount;
		if (data != 0) delete[] data;
		data = new _UCHAR[width * height * byteCount];
		memcpy(data, tmp.data, width * height * byteCount);
	}

	~Bitmap() {
		delete data;
	}

private:

	void read_bmp(string imgName) {

		FILE *  file = fopen(imgName.c_str(), "rb");
		_UCHAR * buf = new _UCHAR[sizeof(bmphead)];
		_UCHAR * bufptr = buf;
		fread(buf, sizeof(bmphead), 1, file);
		bmphead.bfType = READWORD(bufptr);
		bmphead.bfSize = READDWORD(bufptr);
		bmphead.bfReserved1 = READWORD(bufptr);
		bmphead.bfReserved1 = READWORD(bufptr);
		bmphead.bfOffBits = READDWORD(bufptr);
		bmphead.biSize = READDWORD(bufptr);
		bmphead.biWidth = READLONG(bufptr);
		bmphead.biHeight = READLONG(bufptr);
		bmphead.biPlanes = READWORD(bufptr);
		bmphead.biBitCount = READWORD(bufptr);
		bmphead.biCompression = READDWORD(bufptr);
		bmphead.biSizeImage = READDWORD(bufptr);
		bmphead.biXPelsPerMeter = READLONG(bufptr);
		bmphead.biYPelsPerMeter = READLONG(bufptr);
		bmphead.biClrUsed = READDWORD(bufptr);
		bmphead.biClrImportant = READDWORD(bufptr);
		width = bmphead.biWidth;
		height = bmphead.biHeight;
		byteCount = bmphead.biBitCount / 8;
		delete[] buf;
		if (data != 0) delete[] data;

		unsigned int image_data_length = width * height * byteCount;
		data = new _UCHAR[image_data_length];

		fseek(file, bmphead.bfOffBits, SEEK_SET);
		char tmp;
		for (size_t i = 0; i < height; i++)
		{
			//fread(data + width * byteCount * i, width * byteCount, 1, file);
	
			fread(data + width * byteCount * i, sizeof(_UCHAR), width * byteCount, file);
			if ((width * byteCount) % 4) 
				fread(&tmp, sizeof(_UCHAR), 1, file);
				//fseek(file, 1, SEEK_CUR);
		
		}


#ifdef _DEBUG_
		cout << " load image data flom file .bmp" << endl;
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width * byteCount; j++)
			{
				cout << (int)data[i * width * byteCount + j] << " ";
			}
			cout << endl;
		}
#endif // _DEBUG_

	}

	void grayscale() {

		size_t image_data_length = width * height * byteCount;

		_UCHAR * grayimg = new _UCHAR[image_data_length];

		for (size_t i = 0; i < image_data_length - 3; i += 3) {
			grayimg[i] = grayimg[i + 1] = grayimg[i + 2] = (_UCHAR)(data[i] * 0.3 + data[i + 1] * 0.59 + data[i + 2] * 0.11 + 0.01);
		}
		delete[] data;
		data = grayimg;

#ifdef _DEBUG_
		cout << " grayscale image in subpixel " << endl;
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width * byteCount - 3; j += 3)
			{
				cout <<
					(int)data[i * width * byteCount + j + 0] << " " <<
					(int)data[i * width * byteCount + j + 1] << " " <<
					(int)data[i * width * byteCount + j + 2] << " ";
			}
			cout << endl;
		}

#endif // _DEBUG_
	}

	void compact_grayscale() {

		size_t gray_image_data_length = width * height ;
		size_t image_data_length = width * height * byteCount;

		_UCHAR * gray_image = new _UCHAR[gray_image_data_length];

		for (size_t 
			j = 0,                        i = 0; 
			j < gray_image_data_length && i < image_data_length -2; 
			j += 1,                       i += 3) 
		{
			gray_image[j] = (_UCHAR)(data[i] * 0.3 + data[i + 1] * 0.59 + data[i + 2] * 0.11 + 0.01);
		}

		delete[] data;
		byteCount = 1;
		data = gray_image;

#ifdef _DEBUG_
		cout << " grayscale image in subpixel " << endl;
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width * byteCount; j += 1)
			{
				cout << (int)data[i * width * byteCount + j + 0] << " ";
					
			}
			cout << endl;
		}

#endif // _DEBUG_

	}

};
