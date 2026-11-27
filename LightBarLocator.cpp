#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat src = imread("3.jpg");//读入图片3.jpg

    //如果读入为空的就退出
    if(src.empty()){
        printf("could not load image……\n");
        return -1;
    }
    
    Mat gray, mask,
    blank=Mat::zeros(src.size(),src.type());//创建Mat类型的函数
    
    cvtColor(src, gray, COLOR_BGR2GRAY);//把src转化为灰度图像保存为gray

    //筛选阈值，第一个参数为单通道图像，第二个参数为保存的图像，150为阈值，255为填充色，
    //THRESH_BINARY为规则，即大于阈值的像素要填充填充色
    threshold(gray,mask,150,255,THRESH_BINARY);

    imshow("阈值图",mask);

    
    vector<vector<Point>> contours;
    findContours(mask.clone(),contours,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    //判断黑块大小，太小的不会圈画出来
    for(int i=0;i<contours.size();i++){
        //计算面积
        double area= contourArea(contours[i]);
        if(area>110){
            
            RotatedRect rr = minAreaRect(contours[i]);   
            
            Point2f pts[4];  
            rr.points(pts);  
            for(int j=0;j<4;j++){          
                line(src, pts[j], pts[(j+1)%4],Scalar(0,255,0),2,8);
            }        
        }

    }

    imshow("result",src);
    imwrite("MarkLightBars.png",src);

    waitKey(0);//暂停，参数为时间，单位毫秒，0为永久
    destroyAllWindows();//摧毁所有程序
    return 0;
}