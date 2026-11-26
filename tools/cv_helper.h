#pragma once
// ===========================================================================
//  由 tools\cl_build.cmd 用 /FI 自动注入，源码里不需要 #include 本文件。
//
//  它做了两件事，让你的代码跟普通教程完全一样：
//    1) imshow / namedWindow / destroyWindow / moveWindow / setWindowTitle
//       收到中文（UTF-8）窗口名时，自动转成 Windows 能正确显示的编码；
//    2) 程序启动时自动把控制台切成 UTF-8，std::cout 输出中文不乱码。
//
//  所以下面这种“教科书写法”可以直接用，中文标题也正常：
//      imshow("输入窗口", src);
//      namedWindow("结果", WINDOW_AUTOSIZE);
//
//  注意：只对“不加 cv:: 前缀”的调用生效（教程里都是不加的）。
//        如果你写成 cv::imshow("输入窗口", src)，就会绕过自动转码。
// ===========================================================================
#ifdef _WIN32
#include <windows.h>
#include <string>
#include <opencv2/opencv.hpp>

namespace ocvwin {

inline std::string toAnsi(const char* utf8) {
    if (!utf8) return std::string();
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
    if (wlen <= 0) return std::string(utf8);
    std::wstring w(static_cast<size_t>(wlen), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &w[0], wlen);
    int alen = WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (alen <= 0) return std::string(utf8);
    std::string a(static_cast<size_t>(alen), '\0');
    WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, &a[0], alen, nullptr, nullptr);
    if (!a.empty() && a.back() == '\0') a.pop_back();
    return a;
}

namespace detail {
struct ConsoleUtf8 {
    ConsoleUtf8() { SetConsoleOutputCP(CP_UTF8); }
};
inline ConsoleUtf8 g_consoleUtf8;
}  // namespace detail

}  // namespace ocvwin

// ---- 与 OpenCV 同名、参数更精确的重载：自动转码窗口名 ----
inline void imshow(const char* winname, const cv::Mat& mat) {
    cv::imshow(ocvwin::toAnsi(winname), mat);
}
inline void namedWindow(const char* winname, int flags = cv::WINDOW_AUTOSIZE) {
    cv::namedWindow(ocvwin::toAnsi(winname), flags);
}
inline void destroyWindow(const char* winname) {
    cv::destroyWindow(ocvwin::toAnsi(winname));
}
inline void moveWindow(const char* winname, int x, int y) {
    cv::moveWindow(ocvwin::toAnsi(winname), x, y);
}
inline void setWindowTitle(const char* winname, const char* title) {
    cv::setWindowTitle(ocvwin::toAnsi(winname), ocvwin::toAnsi(title));
}

// 中文路径同样要转码：OpenCV 的 imread/imwrite 在 Windows 上不认 UTF-8 路径
inline cv::Mat imread(const char* filename, int flags = cv::IMREAD_COLOR) {
    return cv::imread(ocvwin::toAnsi(filename), flags);
}
inline bool imwrite(const char* filename, const cv::Mat& img,
                    const std::vector<int>& params = std::vector<int>()) {
    return cv::imwrite(ocvwin::toAnsi(filename), img, params);
}

#define CVT(text) ocvwin::toAnsi(text)
#endif
