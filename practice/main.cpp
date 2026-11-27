#include <opencv2/opencv.hpp>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    // 让控制台按 UTF-8 显示，否则中文在 GBK(936) 终端里会变乱码
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::cout << "OpenCV 版本: " << CV_VERSION << std::endl;

    // 生成一张图并画个圆
    cv::Mat img(300, 400, CV_8UC3, cv::Scalar(30, 30, 30));
    cv::circle(img, cv::Point(200, 150), 70, cv::Scalar(0, 200, 255), -1);

    cv::imwrite("test.png", img);            // 写文件
    cv::Mat back = cv::imread("test.png");   // 再读回来，证明 imgcodecs 能用
    std::cout << "读取成功: " << back.cols << "x" << back.rows << std::endl;

    cv::imshow("OpenCV Test", back);         // 弹窗显示
    cv::waitKey(0);                          // 按任意键关闭
    return 0;
}
