#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "BITMAPINFO_1.h"

using namespace std;

#define LENGTH_NAME_BMP 100     //bmp file name 

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

//void readbmp(char fp[],)

int main() {
    char strFile[LENGTH_NAME_BMP] = "lena.bmp";
    IMAGEDATA *imagedata = NULL;
    IMAGEDATA *imagedataOut = NULL;

    FILE *fp, *fpo;
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


    double theta;
    int nwidth, nheight;
    double kx, ky, ox, oy;
    printf("theta:\n");
    scanf("%lf", &theta);
    double hudu = 3.1415 * theta / 180;
    nheight = (int)(width * sin(hudu) + height * cos(hudu));
    nwidth = (int)(width * cos(hudu) + height * sin(hudu));
    printf("%d,%d\n", nheight, nwidth);
    double xyinzi = 1.0 * nwidth / width ;
    printf("cos:%lf\n", sin(hudu));
    double yyinzi = 1.0 * nheight / height;

    imagedataOut = (IMAGEDATA *)malloc( width * height * sizeof(imagedata));

    for(int j = 0; j < nheight; j++) {
        for(int i = 0 ; i < nwidth; i++) {
            (*(imagedataOut + j * width + i)).blue = 0;
        }
    }
    int tempx = 0;
    int tempy = 0;
    int color[2][2];
    double sinx = sin(hudu);
    double cosx = cos(hudu);//提前算出来提高效率
    for(int j = 0; j < height; j++) {
        for(int i = 0 ; i < width; i++) {
            kx = (i * xyinzi - nwidth / 2) * cosx + (j * yyinzi - nheight / 2) * sinx + 128;
            ky = -(i * xyinzi - nwidth / 2) * sinx + (j * yyinzi - nheight / 2) * cosx + 128;
            if((kx >= 0) && (ky >= 0) && (kx < width) && (ky < height)) {
                tempx = (int) kx;
                ox = kx - tempx;
                tempy = (int) ky;
                oy = ky - tempy;
                color[0][0] = (*(imagedata + tempy * width + tempx)).blue ;
                color[1][0] = (*(imagedata + tempy * width + tempx + 1)).blue ;
                color[0][1] = (*(imagedata + (tempy + 1) * width + tempx)).blue ;
                color[1][1] = (*(imagedata + (tempy + 1) * width + tempx + 1)).blue;
                double answer = (1 - ox) * (1 - oy) * color[0][0] + ox * (1 - oy) * color[1][0] +
                                (1 - ox) * oy * color[0][1] + ox * oy * color[1][1];
                int fi = (int) answer;
                (*(imagedataOut + j * width + i)).blue = fi;
            } else {
                (*(imagedataOut + j * width + i)).blue = 255;
            }
        }
    }

    if( (fpo = fopen("out2.bmp", "wb")) == NULL) {
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
        fwrite(&((*(imagedataOut + j * width)).blue), sizeof(BYTE), 1, fpo);
    }

    //getchar();

    fclose(fpo);

    delete [] imagedata;
    delete [] imagedataOut;

}

