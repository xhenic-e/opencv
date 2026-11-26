#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


 void colorspace(Mat &image){
    Mat gray,hsv;
    cvtColor(image,hsv,COLOR_BGR2HSV);
    cvtColor(image,gray,COLOR_BGR2GRAY);//把图片变灰
    imshow("HSV",hsv);
    imshow("灰度",gray);
    imwrite("D:/opencv/open/hsv.png",hsv);//把处理完的图片保存
    imwrite("D:/opencv/open/gray.png",gray);
}

void create0(){//创建空白图像
    Mat m = Mat::zeros(Size(128,128),CV_8UC3);
    /*zeros把所有的值初始化为0，size(128,128)大小为128*128像素的图，CV_8UC3
    指的是八字节，三通道，u为unsigned*/
    m=Scalar(0,255,255);
    // cout << "width:" << m.cols << " height:" << m.rows << " channel:" << m.channels() << endl;
    // cout << m << endl;
    imshow("m",m);
}

void create1(){
    Mat m = Mat::ones(Size(8,8),CV_8UC1);
    cout << "width:" << m.cols << " height:" << m.rows << " channel:" << m.channels() << endl;
    cout << m << endl;
}

void copy(Mat &image){//拷贝
    Mat m1,m2;
    m1 = image.clone();//深拷贝
    image.copyTo(m2);//深拷贝

    imshow("m1",m1);
    waitKey(0);
    imshow("m2",m2);
    waitKey(0);

}

void pixel(Mat &image){//对具体的像素点修改
    int h=image.rows;//rows为高度
    int w=image.cols;//cols为宽度
    int dims=image.channels();//channels为通道数
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            if(dims==1){
                int pv = image.at<uchar>(i,j);//取出数值
                image.at<uchar>(i,j)=255-pv;
            }else{
                Vec3b pv = image.at<Vec3b>(i,j);
                for(int k=0;k<3;k++){
                    image.at<Vec3b>(i,j)[k]=255-pv[k];
                }
            }
        }
    }
}

void sp(Mat &image){//分离输出
    vector<Mat> mv;
    Mat dst;

    split(image,mv);
    mv[1] = 0;
    mv[2] = 0;
    merge(mv,dst);
    imshow("蓝色",dst);

    split(image,mv);
    mv[0] = 0;
    mv[2] = 0;
    merge(mv,dst);
    imshow("绿色",dst);

    split(image,mv);
    mv[0] = 0;
    mv[1] = 0;
    merge(mv,dst);
    imshow("红色",dst);
    
}

void mixc(Mat &image){//通道混合
    Mat dst = image.clone();
    int arr[]={0,1,1,2,2,0};
    mixChannels(&image,1,&dst,1,arr,3);
    imshow("混合",dst);
}

void inrange(Mat &image){//取出绿色
    Mat hsv,mask;
    cvtColor(image,hsv,COLOR_BGR2HSV);
    /*把hsv中属于范围内的像素变为白色，其余变为黑色*/
    inRange(hsv,Scalar(35,43,46),Scalar(77,255,255),mask);
    
    imshow("mask1",mask);
    
    //创建一个空的Mat，尺寸和类型和image一样
    Mat redback=Mat::zeros(image.size(),image.type());
    redback=Scalar(40,40,200);//填色
    bitwise_not(mask,mask);//颜色反转
    imshow("mask2",mask);
    /*mask 中为白色(255)的位置：把 image 的像素拷贝到 redback；
    mask 中为黑色(0)的位置：不拷贝，redback 保持原来的颜色*/
    image.copyTo(redback,mask);
    
    imshow("redback",redback);
    
}

void minmax(Mat &image){//查找最大最小值，平均值，方差
    double minv,maxv;//minv为单通道的最小值，maxv为单通道的最大值
    Point minloc,maxloc;//minloc为最小值的位置，mzxloc为最大值的位置，
    vector<Mat> mv;
    split(image,mv);
    for(int i=0;i<mv.size();i++){
        minMaxLoc(mv[i],&minv,&maxv,&minloc,&maxloc,Mat());
        cout << i << ":min:" << minv << " max:" << maxv << " minloc:" << minloc << " maxloc:" <<
        maxloc << endl; 
    }
    Mat mean,stddev;//mean为平均值，stddev为方差
    meanStdDev(image,mean,stddev);
    cout << "mean:" << mean << " stddev:" << stddev << endl; 
}

void drawrect(Mat &image){//绘制矩形
    Rect rect={750,220,200,200};
    rectangle(image,rect,Scalar(0,0,255),2,8);
    imshow("绘制矩形",image);
}

void drawcircle(Mat &image){//绘制圆形
    circle(image,Point(200,200),100,Scalar(0,255,0),-1,8);
    imshow("绘制圆形",image);
}

void addw(Mat &image){//绘制矩形并合并图像
    Mat bg(image.size(),image.type()),dst;
    rectangle(bg,Rect(750,220,200,200),Scalar(0,0,255),-1,8);
    imshow("矩形",bg);
    addWeighted(image,0.5,bg,0.5,0,dst);
    imshow("合并图像",dst);
}

void drawline(Mat &image){//绘制直线
    Mat bg(image.size(),image.type());
    line(bg,Point(200,200),Point(500,500),Scalar(0,0,255),2,8);
    imshow("绘制直线",bg);
}

void drawellipse(Mat &image){//绘制椭圆
    Mat bg(image.size(),image.type());
    RotatedRect rrt;
    rrt.center=Point(500,500);//中心点
    rrt.size=Size(200,10);//尺寸，第一个数是b，第二个数是a
    rrt.angle=0.0;//角度
    ellipse(bg,rrt,Scalar(0,255,255),2,8);
    imshow("绘制椭圆",bg); 
}

void ramdon_draw(){
    Mat canvas=Mat::zeros(Size(512,512),CV_8UC3);
    RNG rng(12345);
    int w = canvas.cols;
    int h = canvas.rows;
    while(true){
        int c=waitKey(10);
        if(c==27){
            break;
        }
        int x1=rng.uniform(0,w);
        int y1=rng.uniform(0,h);
        int x2=rng.uniform(0,w);
        int y2=rng.uniform(0,h);
        int b=rng.uniform(0,256);
        int g=rng.uniform(0,256);
        int r=rng.uniform(0,256);
        canvas=Scalar(0,0,0);
        line(canvas,Point(x1,y1),Point(x2,y2),Scalar(b,g,r),2,8);
        imshow("随即绘制演示",canvas);
    }
}

int main() {
    Mat src = imread("shenzi.png");
    // Mat src = imread("shenzi.png",IMREAD_GRAYSCALE);//读入一张图片,变灰

    //如果读入为空的就退出
    if(src.empty()){
        printf("could not load image……\n");
        return -1;
    }

    // namedWindow("输入窗口", WINDOW_FREERATIO);//自由调节窗口
    // imshow("输入窗口", src);//展示图片

    // colorspace(src);

    // create0();

    // copy(src);

    // pixel(src);
    // namedWindow("反转", WINDOW_FREERATIO);
    // imshow("反转",src);

    // sp(src);

    // mixc(src);

    // inrange(src);

    // minmax(src);

    // drawrect(src);

    // drawcircle(src);

    // addw(src);

    // drawline(src);

    // drawellipse(src);

    ramdon_draw();


    waitKey(0);//暂停，参数为时间，单位毫秒，0为永久
    destroyAllWindows();//摧毁所有程序
    return 0;
}
