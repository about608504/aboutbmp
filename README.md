# aboutbmp

256灰度的bmp图像的调色板里面，同一灰度rgb数值相同，故只使用blue来表示灰度   
##2018.4.16
在opencv环境下利用霍夫变换检测直线，并计算角度将票据图像旋转
##2018.3.28  
添加了gitignore删除了一些缓存和exe文件  
homework7中值滤波器  
## 2018.3.27  
分模块实现  
优化homework6的一些细节
了解透视变换和仿射变换
https://blog.csdn.net/xieyan0811/article/details/71106539
仿射变换后平行四边形的各边仍操持平行，透视变换结果允许是梯形等四边形，所以仿射变换是透视变换的子集  

## 2018.3.20   
实现了双线性插值旋转的操作  
比较了每次写一行和逐像素写的差距大约在两倍左右  

## 2018.3.14  
实现了bmp图像的点运算，二值化，平移后相减的操作  
对应ppt上面homework4/5  

## 2018.3.7  
实现了直接阅读BMP头文件来获取bmp图像的信息。并直方图均衡化  
win下运行正常。  
i,j为直角坐标系图像左下角为原点。  
图像数值255为白色，0为黑色。 
