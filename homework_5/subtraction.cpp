#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <time.h>
#include <algorithm>
#include "BITMAPINFO_1.h"
struct timeb t1, t2; //计算程序运行时间
using namespace std;

#define LENGTH_NAME_BMP 100     //bmp file name 

BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;
BITMAPFILEHEADER strHead_1;
BITMAPINFOHEADER strInfo_1;
RGBQUAD strPalette[256];
RGBQUAD strPalette_1[256];

int main() {
    char strFile[LENGTH_NAME_BMP] = "lena.bmp";
    char strFile_1[LENGTH_NAME_BMP] = "lena_1.bmp";
    IMAGEDATA *imagedata = NULL;
    IMAGEDATA *imagedataOut = NULL;
    IMAGEDATA *imagedata_1 = NULL;
    IMAGEDATA *imagedataOut_1 = NULL;
    int width;
    int height;
    int width_1;
    int height_1;
    FILE *fp, *fpo, *fp_1, *fpo_1;
    fp = fopen(strFile, "rb");
    fp_1 = fopen(strFile_1, "rb");


    if( fp != NULL ) {
        WORD bfType;    //the type of the file
        fread(&bfType, sizeof(WORD), 1, fp);

        if( 0x4d42 != bfType ) {
            cout << "The file1 is not bmp!" << endl;
            return 0;
        }
        fread(&strHead, sizeof(tagBITMAPFILEHEADER), 1, fp);
        fread(&strInfo, sizeof(tagBITMAPINFOHEADER), 1, fp);

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
        imagedataOut = (IMAGEDATA *)malloc( width * height * sizeof(imagedata));

        for(int j = 0; j < height; j++) {
            for(int i = 0 ; i < width; i++) {
                (*(imagedata + j * width + i)).blue = 0;
                (*(imagedataOut + j * width + i)).blue = 0;
            }
        }

        //读出图片的像素数据
        fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fp);
        fclose(fp);
    } else {
        cout << "File1 open error!" << endl;
        return 0;
    }

    if( fp_1 != NULL ) {
        WORD bfType_1;  //the type of the file
        fread(&bfType_1, sizeof(WORD), 1, fp_1);

        if( 0x4d42 != bfType_1 ) {
            cout << "The file2 is not bmp!" << endl;
            return 0;
        }
        fread(&strHead_1, sizeof(tagBITMAPFILEHEADER), 1, fp_1);
        fread(&strInfo_1, sizeof(tagBITMAPINFOHEADER), 1, fp_1);

        //读取调色板
        for(int i = 0; i < strInfo_1.biClrUsed; i++) {
            fread((char *) & (strPalette_1[i].rgbBlue), sizeof(BYTE), 1, fp_1);
            fread((char *) & (strPalette_1[i].rgbGreen), sizeof(BYTE), 1, fp_1);
            fread((char *) & (strPalette_1[i].rgbRed), sizeof(BYTE), 1, fp_1);
            fread((char *) & (strPalette_1[i].rgbReserved), sizeof(BYTE), 1, fp_1);
        }

        width_1 = strInfo_1.biWidth;
        height_1 = strInfo_1.biHeight;
        //width_1 = (width_1 * sizeof(IMAGEDATA) + 3) /4 * 4;
        width_1  = ((width_1 % 4 == 0  ? width_1 : width_1 + (4 - (width_1 % 4))) * strInfo_1.biBitCount) / 8;
        imagedata_1 = (IMAGEDATA *)malloc( width_1 * height_1 );

        for(int j = 0; j < height_1; j++) {
            for(int i = 0 ; i < width_1; i++) {
                (*(imagedata_1 + j * width_1 + i)).blue = 0;
            }
        }

        //读出图片的像素数据
        fread(imagedata_1, sizeof(struct tagIMAGEDATA) * width_1, height_1, fp_1);
        fclose(fp_1);
    } else {
        cout << "File2 open error!" << endl;
        return 0;
    }


    for(int j = 0; j < height; j++) {
        for(int i = 0 ; i < width; i++) {
            (*(imagedataOut + j * width + i)).blue = (*(imagedata + j * width + i)).blue - (*(imagedata_1 + j * width + i)).blue;
        }
    }



    ftime(&t1);
    for(int k = 0; k <= 1; k++) {
        if( (fpo = fopen("out_2.bmp", "wb")) == NULL) {
            free(imagedata);
            free(imagedataOut);
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
            //for(int i = 0; i < width; i++) {
            fwrite(&((*(imagedataOut + j * width)).blue), sizeof(BYTE)*width, 1, fpo);
            //}
        }
        fclose(fpo);
    }
    ftime(&t2);
    cout << (t2.time - t1.time) * 1000 + (t2.millitm - t1.millitm) << "ms" << endl;


    // ftime(&t1);
    // for(int k = 0; k <= 5000; k++) {
    //     if( (fpo = fopen("out_2.bmp", "wb")) == NULL) {
    //         free(imagedata);
    //         free(imagedataOut);
    //         cout << "Create the bmp file error!" << endl;
    //         return 0;
    //     }

    //     WORD bfTypeOut = 0x4d42;
    //     fwrite(&bfTypeOut, sizeof(WORD), 1, fpo);
    //     fwrite(&strHead, sizeof(tagBITMAPFILEHEADER), 1, fpo);
    //     strInfo.biHeight = height;
    //     strInfo.biWidth = width;
    //     fwrite(&strInfo, sizeof(tagBITMAPINFOHEADER), 1, fpo);

    //     //保存调色板数据
    //     for(int i = 0 ; i < strInfo.biClrUsed; i++) {
    //         fwrite(&strPalette[i].rgbBlue, sizeof(BYTE), 1, fpo);
    //         fwrite(&strPalette[i].rgbGreen, sizeof(BYTE), 1, fpo);
    //         fwrite(&strPalette[i].rgbRed, sizeof(BYTE), 1, fpo);
    //         fwrite(&strPalette[i].rgbReserved, sizeof(BYTE), 1, fpo);
    //     }

    //     //保存像素数据


    //     for(int j = 0; j < height; j++) {
    //         for(int i = 0; i < width; i++) {
    //             fwrite(&((*(imagedataOut + j * width + 1)).blue), sizeof(BYTE), 1, fpo);
    //         }
    //     }
    //     fclose(fpo);
    // }
    // ftime(&t2);
    // cout << (t2.time - t1.time) * 1000 + (t2.millitm - t1.millitm) << "ms" << endl;






    // ftime(&t1);
    // for(int k = 0; k <= 1000; k++) {

    //     for(int j = 0; j < height; j++) {
    //         for(int i = 0; i < width; i++) {
    //         fwrite(&((*(imagedataOut + j * width + i)).blue), sizeof(BYTE), 1, fpo);
    //         }
    //     }
    // }
    // ftime(&t2);
    // cout << (t2.time - t1.time) * 1000 + (t2.millitm - t1.millitm) << "ms" << endl;




    free(imagedata);
    free(imagedataOut);

}




