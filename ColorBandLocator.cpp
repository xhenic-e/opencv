#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat src = imread("2.png");//读入图片2.png

    //如果读入为空的就退出
    if(src.empty()){
        printf("could not load image……\n");
        return -1;
    }
    
    Mat hsv, mask1, mask2;//创建Mat类型的函数
    cvtColor(src, hsv, COLOR_BGR2HSV);//把src转化为HSV图像保存为hsv
    inRange(hsv, Scalar(100,100,100), Scalar(130,255,255), mask1);  
    //识别蓝色范围，把hsv中属于范围内的像素变为白色，其余变为黑色
    // 白色=找到的蓝色
    imshow("1. 蓝色掩码", mask1); //imshow显示图片     

    vector<vector<Point>> blue_contours;//创建一个contours存放蓝色边界的点
    //.clone()克隆一个mask1防止破坏mask1
    findContours(mask1.clone(), blue_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    printf("找到 %d 个蓝色轮廓\n", (int)blue_contours.size());

    Mat blue_result = src.clone();
    drawContours(blue_result, blue_contours, -1, Scalar(0,255,255),2,8);  //黄色描边
    imshow("2. 黄色轮廓", blue_result);

    //处理红色
    inRange(hsv, Scalar(0,43,46), Scalar(10,255,255), mask2);
    imshow("3. 红色掩码", mask2);

    vector<vector<Point>> red_contours;
    findContours(mask2.clone(), red_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    printf("找到 %d 个红色轮廓\n", (int)red_contours.size());

    Mat redblue_result = blue_result.clone();
    drawContours(redblue_result, red_contours, -1, Scalar(0,255,0), 2,8);  // 绿色描边
    imshow("4. 绿色轮廓", redblue_result);

    moveWindow("1. 蓝色掩码", 50, 50);
    moveWindow("2. 黄色轮廓", 50, 300);
    moveWindow("3. 红色掩码", 500, 50);
    moveWindow("4. 绿色轮廓", 500, 300);

    imwrite("redblue_result.png",redblue_result);//保存处理后的图片
    

    waitKey(0);//暂停，参数为时间，单位毫秒，0为永久
    destroyAllWindows();//摧毁所有程序
    return 0;
}