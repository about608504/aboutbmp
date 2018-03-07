#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "BITMAPINFO_1.h"

using namespace std;

#define LENGTH_NAME_BMP 100		//bmp file name 

BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;
RGBQUAD strPalette[256];

void showBmpHead(BITMAPFILEHEADER pBmpHead) {
    cout << "the bitmap header file: " << endl;
    cout << "the size of bitmap: " << pBmpHead.bfSize << endl;
    cout << "Reserved 1: " << pBmpHead.bfReserved1 << endl;
    cout << "Reserved 2: " << pBmpHead.bfReserved2 << endl;
    cout << "shifted number of bytes in image data: " << pBmpHead.bfoffBits << endl << endl;
}

void showBmpInfoHead(tagBITMAPINFOHEADER pBmpInfoHead) {
    cout << "the bitmap info header: " << endl;
    cout << "length of struct: " << pBmpInfoHead.biSize << endl;
    cout << "width of bitmap: " << pBmpInfoHead.biWidth << endl;
    cout << "height of bitmap: " << pBmpInfoHead.biHeight << endl;
    cout << "number of bit planes: " << pBmpInfoHead.biPlanes << endl;
    cout << "biBitCount: " << pBmpInfoHead.biBitCount << endl;
    cout << "compression method: " << pBmpInfoHead.biCompression << endl;
    cout << "biSizeImage: " << pBmpInfoHead.biSizeImage << endl;
    cout << "X resolution: " << pBmpInfoHead.biXPelsPerMeter << endl;
    cout << "Y resolution: " << pBmpInfoHead.biYPelsPerMeter << endl;
    cout << "the number of used colors: " << pBmpInfoHead.biClrUsed << endl;
    cout << "important colors: " << pBmpInfoHead.biClrImportant << endl;
}

int main() {
    char strFile[LENGTH_NAME_BMP] = "lena.bmp";
    IMAGEDATA *imagedata = NULL;
    IMAGEDATA *imagedataOut = NULL;
    int width;
    int height;
    FILE *fp, *fpo;
    fp = fopen(strFile, "rb");

    if( fp != NULL ) {
        WORD bfType;	//the type of the file
        fread(&bfType, sizeof(WORD), 1, fp);

        if( 0x4d42 != bfType ) {
            cout << "The file is not bmp!" << endl;
            return 0;
        }

        fread(&strHead, sizeof(tagBITMAPFILEHEADER), 1, fp);
        showBmpHead(strHead);
        fread(&strInfo, sizeof(tagBITMAPINFOHEADER), 1, fp);
        showBmpInfoHead(strInfo);

        //读取调色板
        for(int i = 0; i < strInfo.biClrUsed; i++) {
            fread((char *) & (strPalette[i].rgbBlue), sizeof(BYTE), 1, fp);
            fread((char *) & (strPalette[i].rgbGreen), sizeof(BYTE), 1, fp);
            fread((char *) & (strPalette[i].rgbRed), sizeof(BYTE), 1, fp);
            fread((char *) & (strPalette[i].rgbReserved), sizeof(BYTE), 1, fp);
        }
        //int bbb = 254;
        //printf("%d %d %d %d",strPalette[bbb].rgbBlue,strPalette[bbb].rgbGreen,strPalette[bbb].rgbRed,strPalette[bbb].rgbReserved);

        width = strInfo.biWidth;
        height = strInfo.biHeight;
        //width = (width * sizeof(IMAGEDATA) + 3) /4 * 4;
        width  = ((width % 4 == 0  ? width : width + (4 - (width % 4))) * strInfo.biBitCount) / 8;
        imagedata = (IMAGEDATA *)malloc( width * height );
        //printf("new width:%d",width);
        //BYTE bcy = 1;
        //printf("sizeof bcy:%d",sizeof(IMAGEDATA *));
        imagedataOut = (IMAGEDATA *)malloc( width * height * sizeof(imagedata));

        for(int j = 0; j < height; j++) {
            for(int i = 0 ; i < width; i++) {
                (*(imagedata + j * width + i)).blue = 0;
                (*(imagedataOut + j * width + i)).blue = 0;
            }
        }

        //读出图片的像素数据
        fread(imagedata, sizeof(IMAGEDATA) * width, height, fp);
        fclose(fp);
    } else {
        cout << "File open error!" << endl;
        return 0;
    }

    for(int j = 0; j < height; j++) {
        for(int i = 0 ; i < width; i++) {
            *(imagedataOut + j * width + i) = *(imagedata + j * width + i);           
        }
    }
    for(int j = 0; j < 20; j++) {
        for(int i = 0 ; i < 10; i++) {
            printf("%d ",(*(imagedataOut + j * width + i)).blue);
        }
        printf("\n");
    }

    if( (fpo = fopen("out.bmp", "wb")) == NULL) {
        cout << "Create the bmp file error!" << endl;
        return 0;
    }

    WORD bfTypeOut = 0x4d42;
    fwrite(&bfTypeOut, sizeof(WORD), 1, fpo);
    fwrite(&strHead, sizeof(tagBITMAPFILEHEADER), 1, fpo);
    strInfo.biHeight = height;
    strInfo.biWidth = width;
    fwrite(&strInfo, sizeof(tagBITMAPINFOHEADER), 1, fpo);

    //保存调色板数据
    for(int i = 0 ; i < strInfo.biClrUsed; i++) {
        fwrite(&strPalette[i].rgbBlue, sizeof(BYTE), 1, fpo);
        fwrite(&strPalette[i].rgbGreen, sizeof(BYTE), 1, fpo);
        fwrite(&strPalette[i].rgbRed, sizeof(BYTE), 1, fpo);
        fwrite(&strPalette[i].rgbReserved, sizeof(BYTE), 1, fpo);
    }

    //保存像素数据
    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            fwrite(&((*(imagedataOut + j * width + i)).blue), sizeof(BYTE), 1, fpo);
        }
    }

    //getchar();

    fclose(fpo);

    delete [] imagedata;
    delete [] imagedataOut;

}
