#ifndef BITMAPINFO_H
#define BITMAPINFO_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//bitmap file header
typedef struct tagBITMAPFILEHEADER
{
	//WORD bfType;		//�ļ����ͣ�������0x424D�����ַ���BM��  
	DWORD bfSize;		//�ļ���С
	WORD bfReserved1;	//������
	WORD bfReserved2;	//������
	DWORD bfoffBits;	//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
}BITMAPFILEHEADER;

//bitmap information head
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;			//��Ϣͷ��С
	LONG biHeight;			//ͼ����
	LONG biWidth;			//ͼ��߶�
	WORD biPlanes;			//λƽ����������Ϊ1
	WORD biBitCount;		//����λ��
	DWORD biCompression;	//ѹ������
	DWORD biSizeImage;		//ѹ��ͼ���С�ֽ���
	LONG biXPelsPerMeter;	//ˮƽ�ֱ���
	LONG biYPelsPerMeter;	//��ֱ�ֱ���
	DWORD biClrUsed;		//λͼʵ���õ���ɫ����
	DWORD biClrImportant;	//��λͼ����Ҫ��ɫ����
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