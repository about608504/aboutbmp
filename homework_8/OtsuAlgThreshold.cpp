#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "BITMAPINFO_1.h"

using namespace std;

#define LENGTH_NAME_BMP 100     //bmp file name
IMAGEDATA *imagedata = NULL;
IMAGEDATA *imagedataOut = NULL;
BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;
RGBQUAD strPalette[256];
int width;
int height;
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
int LoadBmpFile(char *strFile) {

    FILE *fp;
    fp = fopen(strFile, "rb");
    if( fp != NULL ) {
        WORD bfType;    //the type of the file
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

        width = strInfo.biWidth;
        height = strInfo.biHeight;
        //width = (width * sizeof(IMAGEDATA) + 3) /4 * 4;
        width  = ((width % 4 == 0  ? width : width + (4 - (width % 4))) * strInfo.biBitCount) / 8;
        imagedata = (IMAGEDATA *)malloc( width * height );


        for(int j = 0; j < height; j++) {
            for(int i = 0 ; i < width; i++) {
                (*(imagedata + j * width + i)).blue = 0;
                //(*(imagedataOut + j * width + i)).blue = 0;
            }
        }
        //读出图片的像素数据
        fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fp);
        fclose(fp);
    } else {
        cout << "File open error!" << endl;
        return 0;
    }
}

int Otsu() {
    imagedataOut = (IMAGEDATA *)malloc( width * height * sizeof(imagedata));

    for(int j = 0; j < height; j++) {
        for(int i = 0 ; i < width; i++) {
            (*(imagedataOut + j * width + i)).blue = 0;
        }
    }
    int T = 0; //Otsu算法阈值
    double varValue = 0; //类间方差中间值保存
    double w0 = 0; //前景像素点数所占比例
    double w1 = 0; //背景像素点数所占比例
    double u0 = 0; //前景平均灰度
    double u1 = 0; //背景平均灰度
    double Histogram[256] ={0};
    double totalNum = height * width; //像素总数
    
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            Histogram[(*(imagedata + j * width + i)).blue]=Histogram[(*(imagedata + j * width + i)).blue]+1;
        }
    }
	printf("\n");
    for(int i = 0; i < 255; i++) {
        w1 = 0;
        u1 = 0;
        w0 = 0;
        u0 = 0;
        for(int j = 0; j <= i; j++) {
            w1 = w1 + Histogram[j]; //背景部分像素点总数
            u1 = u1 + j * Histogram[j];
        }
        if(w1 == 0) {
            continue;
        }
        u1 = u1 / w1; //背景像素平均灰度
        w1 = w1 / totalNum;
        
        for(int k = i + 1; k < 255; k++) {
            w0 += Histogram[k]; //前景部分像素点总数
            u0 += k * Histogram[k]; //前景部分像素总灰度和
        }
        if(w0 == 0) { //前景部分像素点数为0时退出
            break;
        }
        u0 = u0 / w0; //前景像素平均灰度
        w0 = w0 / totalNum; // 前景部分像素点数所占比例

        double varValueI = w0 * w1 * (u1 - u0) * (u1 - u0); //当前类间方差计算
        if(varValue < varValueI) {
            varValue = varValueI;
            T = i;
        }
    }
    printf("T:%d\n", T);
    for(int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if((*(imagedata + j * width + i)).blue > T) {
                (*(imagedataOut + j * width + i)).blue = 255;
            } else {
                (*(imagedataOut + j * width + i)).blue = 0;
            }
        }
    }
    return 0;
}


int SaveImg(char *outpath) {
    FILE *fpo;
    if( (fpo = fopen(outpath, "wb")) == NULL) {
        cout << "Create the bmp file error!" << endl;
        free(imagedata);
        free(imagedataOut);
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
        fwrite(&((*(imagedataOut + j * width)).blue), sizeof(BYTE) * width, 1, fpo);
    }

    //getchar();

    fclose(fpo);
}
int main() {
    char path[LENGTH_NAME_BMP] = "lena.bmp";
    char wirpath[LENGTH_NAME_BMP] = "out.bmp";
    LoadBmpFile(path);
    Otsu();

    SaveImg(wirpath);
    free(imagedata);
    free(imagedataOut);
}
