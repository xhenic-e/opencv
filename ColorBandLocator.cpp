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
    
    Mat hsv, mask1, mask2;
    cvtColor(src, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(100,100,100), Scalar(130,255,255), mask1);  // 蓝色范围

    imshow("1. 蓝色掩码", mask1);      // 白色 = 找到的蓝色

    vector<vector<Point>> blue_contours;
    findContours(mask1.clone(), blue_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    printf("找到 %d 个蓝色轮廓\n", (int)blue_contours.size());

    Mat blue_result = src.clone();
    drawContours(blue_result, blue_contours, -1, Scalar(0,255,255), 2,LINE_AA);  // 黄色描边
    imshow("2. 黄色轮廓", blue_result);

    inRange(hsv, Scalar(0,43,46), Scalar(10,255,255), mask2);
    imshow("3. 红色掩码", mask2);

    vector<vector<Point>> red_contours;
    findContours(mask2.clone(), red_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    printf("找到 %d 个红色轮廓\n", (int)red_contours.size());

    Mat redblue_result = blue_result.clone();
    drawContours(redblue_result, red_contours, -1, Scalar(0,255,0), 2,LINE_AA);  // 绿色描边
    imshow("4. 绿色轮廓", redblue_result);

    waitKey(0);//暂停，参数为时间，单位毫秒，0为永久
    destroyAllWindows();//摧毁所有程序
    return 0;
}