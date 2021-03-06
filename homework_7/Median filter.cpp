#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "BITMAPINFO_1.h"
#include <algorithm>
using namespace std;

#define LENGTH_NAME_BMP 100     //bmp file name
IMAGEDATA *imagedata = NULL;
IMAGEDATA *imagedataOut = NULL;
BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;
RGBQUAD strPalette[256];
int width;
int height;
vector<int> obj;
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

int MedianFilter(int size) {
    imagedataOut = (IMAGEDATA *)malloc( width * height * sizeof(imagedata));
    for(int j = 0; j < height; j++) {
        for(int i = 0 ; i < width; i++) {
            (*(imagedataOut + j * width + i)).blue = 0;
        }
    }
    //中值滤波
    int k = size;
    for(int j = 0; j < height - k + 1; j++) {
        for(int i = 0 ; i < width - k + 1; i++) {
            for(int ii = i; ii < i + k; ii++) {
                for(int jj = j; jj < j + k; jj++) {
                    obj.push_back((*(imagedata + jj * width + ii)).blue);
                }
            }
            sort(obj.begin(), obj.end());
            WORD value = (WORD)obj[(size * size - 1) / 2];

            (*(imagedataOut +  (j + (k - 1) / 2)* width + (i + (k - 1) / 2))).blue = value;

            obj.clear();
        }
    }
    //边缘填上原来的
    int half = (k - 1) / 2;
    for(int j = 0; j < height; j++) {
        for(int i = 0 ; i < width ; i++) {
            if ((i < half) || (i >= width - half) || (j < half) || (j >= height - half)) {
                (*(imagedataOut + j * width + i)).blue = (*(imagedata + j * width + i)).blue;
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
    fclose(fpo);
}


int main() {
    char path[LENGTH_NAME_BMP] = "lena.bmp";
    char wirpath[LENGTH_NAME_BMP] = "out2.bmp";
    LoadBmpFile(path);
    int temp = 0;
    printf("size:\n");
    scanf("%d", &temp);
    MedianFilter(temp);
    SaveImg(wirpath);
    free(imagedata);
    free(imagedataOut);
}
