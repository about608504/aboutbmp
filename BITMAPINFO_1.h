#ifndef BITMAPINFO_H
#define BITMAPINFO_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//bitmap file header
typedef struct tagBITMAPFILEHEADER
{
	//WORD bfType;		//文件类型，必须是0x424D，即字符“BM”  
	DWORD bfSize;		//文件大小
	WORD bfReserved1;	//保留字
	WORD bfReserved2;	//保留字
	DWORD bfoffBits;	//从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER;

//bitmap information head
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;			//信息头大小
	LONG biHeight;			//图像宽度
	LONG biWidth;			//图像高度
	WORD biPlanes;			//位平面数，必须为1
	WORD biBitCount;		//像素位数
	DWORD biCompression;	//压缩类型
	DWORD biSizeImage;		//压缩图像大小字节数
	LONG biXPelsPerMeter;	//水平分辨率
	LONG biYPelsPerMeter;	//垂直分辨率
	DWORD biClrUsed;		//位图实际用到的色彩数
	DWORD biClrImportant;	//本位图中重要的色彩数
}BITMAPINFOHEADER;

//bitmap palette/color table
typedef struct tagRGBQUAD
{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
}RGBQUAD;

//bitmap data
typedef struct tagIMAGEDATA
{
	BYTE blue;
	//BYTE green;
	//BYTE red;
}IMAGEDATA;

#endif