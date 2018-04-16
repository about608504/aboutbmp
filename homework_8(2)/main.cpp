#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
using namespace cv;
using namespace std;
int main( )
{
    Mat srcImage = imread("/Users/bcy/Desktop/123.png");//路径需要修改
    Mat midImage,dstImage;
    Canny(srcImage, midImage, 100, 200, 3);
    cvtColor(midImage,dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图
    vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
    vector<double> theta_true;
    HoughLines(midImage, lines, 1, CV_PI/180, 150, 0, 0 );
    for( int i = 0; i < lines.size(); i++ )
    {
        double rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);//画出直线
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( dstImage, pt1, pt2, Scalar(55,100,195), 1, CV_AA);
        theta_true.push_back(theta/3.14*180-90);
    }
    std::sort(theta_true.begin(), theta_true.end());
    double  angle = theta_true[lines.size()/2];//尽量检测到的直线少一些，这样取出的中位数大概就是旋转的角度
    cout<< angle <<endl;
    Point2f center(srcImage.cols/2,srcImage.rows/2);
    Mat M = getRotationMatrix2D(center, angle, 1);
    cout<< M <<endl;
    Mat MoveImage(srcImage.rows,srcImage.cols,CV_8UC3,Scalar(0,0,0));
    warpAffine(srcImage, MoveImage, M, Size(srcImage.cols,srcImage.rows));//仿射变换
    circle(MoveImage, center, 2, Scalar(255,0,0));
    imshow("【效果图】", MoveImage);
    waitKey(0);
    
    return 0;
}
