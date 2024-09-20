# 视觉组第二次作业
总体思路：
先用GPT写一个，有对Opencv函数和功能的了解，再具体去查每个函数的作用、格式，修改gpt的错误，优化思路；

资料来源：

https://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/table_of_content_core/table_of_content_core.html；

https://blog.csdn.net/xddwz/article/details/111283121；

https://blog.csdn.net/zhouzongzong/article/details/124334524

https://blog.csdn.net/jndingxin/article/details/108092736

GPT对话：

![Screenshot 1](assets/Screenshot%20from%202024-09-20%2015-52-58.png)

![Screenshot 2](assets/Screenshot%20from%202024-09-20%2015-53-07.png)

![Screenshot 3](assets/Screenshot%20from%202024-09-20%2015-53-10.png)

GPT初始代码：
```cpp
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 读取图像
    Mat img = imread("../resources/test_image.png");
    if (img.empty()) {
        cout << "无法加载图像" << endl;
        return -1;
    }

    // 转换为灰度图
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    imwrite("../resources/gray_image.png", gray);

    // 转换为 HSV 图像
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    imwrite("../resources/hsv_image.png", hsv);

    // 均值滤波
    Mat meanBlur;
    blur(img, meanBlur, Size(5, 5));
    imwrite("../resources/mean_blur_image.png", meanBlur);

    // 高斯滤波
    Mat gaussBlur;
    GaussianBlur(img, gaussBlur, Size(5, 5), 0);
    imwrite("../resources/gauss_blur_image.png", gaussBlur);

    // 提取红色区域
    Mat mask;
    inRange(hsv, Scalar(0, 120, 70), Scalar(10, 255, 255), mask);
    Mat red_area;
    bitwise_and(img, img, red_area, mask=mask);
    imwrite("../resources/red_area.png", red_area);

    // 轮廓检测与绘制
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    Mat contour_output = img.clone();
    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(contour_output, contours, (int)i, Scalar(0, 0, 255), 2, LINE_8, hierarchy, 0);
        // 输出轮廓面积
        cout << "轮廓 " << i << " 的面积: " << contourArea(contours[i]) << endl;
    }
    imwrite("../resources/contours.png", contour_output);

    // 图像旋转
    Point2f center(img.cols/2.0, img.rows/2.0);
    Mat rotationMat = getRotationMatrix2D(center, 35, 1.0);
    Mat rotated;
    warpAffine(img, rotated, rotationMat, img.size());
    imwrite("../resources/rotated_image.png", rotated);

    // 图像裁剪
    Rect roi(0, 0, img.cols / 2, img.rows / 2);
    Mat cropped = img(roi);
    imwrite("../resources/cropped_image.png", cropped);

    return 0;
}

```
后续在该代码的基础上对错误进行了更改并按自己的理解进行了优化，详见main.cpp