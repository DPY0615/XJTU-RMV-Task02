#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 检测是否成功读入图像矩阵
    Mat img = imread("../resources/test_image.png");
    if (img.empty()) {
        cout << "无法读取图像!" << endl;
        return -1;  
    } else {
        cout << "读取图像成功!" << endl;
    }

    // 1. 图像颜色空间转换
    // 转换为灰度图
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    imwrite("../resources/gray_image.png", gray);

    // 转换为HSV图像
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    imwrite("../resources/hsv_image.png", hsv);

    // 2. 滤波操作
    // 均值滤波
    Mat averageBlur;
    blur(img, averageBlur, Size(5, 5));
    imwrite("../resources/average_blur_image.png", averageBlur);

    // 高斯滤波
    Mat gaussBlur;
    GaussianBlur(img, gaussBlur, Size(5, 5), 0, 0);
    imwrite("../resources/gauss_blur_image.png", gaussBlur);

    // 3.特征提取 
    /* 
        提取红色区域
        由于纯红色结果不理想，这里将红色区间扩展至酒红色区间，以得到更加明显的效果；
        绝对不是偷懒喵！；
    */
    Mat mask1, mask2, mask;
    inRange(hsv, Scalar(0, 120, 70), Scalar(10, 255, 255), mask1);  // 红色区间1
    inRange(hsv, Scalar(160, 120, 70), Scalar(180, 255, 255), mask2);  // 红色区间2（包含酒红色）
    mask = mask1 | mask2;  // 合并两个区间的掩膜

    Mat red_area;
    bitwise_and(img, img, red_area, mask=mask);
    imwrite("../resources/red_area.png", red_area);

    // 寻找红色区域的轮廓和bounding box，并计算红色区域的面积
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    Mat contour_output = img.clone();

    double total_red_area = 0.0;

    for (size_t i = 0; i < contours.size(); i++) {
        // 累加每个轮廓的面积
        total_red_area += contourArea(contours[i]);

        // 绘制轮廓
        drawContours(contour_output, contours, (int)i, Scalar(0, 255, 0), 2, LINE_8, hierarchy, 0);

        // 绘制bounding box
        Rect bounding_box = boundingRect(contours[i]);
        rectangle(contour_output, bounding_box, Scalar(255, 0, 0), 2);
    }
    
    cout << "红色区域总面积: " << total_red_area << endl;
    imwrite("../resources/contours_with_bounding_box.png", contour_output);

    // 高亮区域处理
    // 二值化
    Mat binary;
    threshold(gray, binary, 120, 255, THRESH_BINARY); //直接使用之前灰图结果，省去灰度化
    imwrite("../resources/binary_image.png", binary);

    // 膨胀
    Mat dilated;
    dilate(binary, dilated, Mat(), Point(-1, -1), 2);
    imwrite("../resources/dilated_image.png", dilated);

    // 腐蚀
    Mat eroded;
    erode(dilated, eroded, Mat(), Point(-1, -1), 2);
    imwrite("../resources/eroded_image.png", eroded);

    // 漫水填充（填充白色区域）
    Mat floodfilled = eroded.clone();
    floodFill(floodfilled, Point(0, 0), Scalar(255));
    imwrite("../resources/floodfilled_image.png", floodfilled);

    // 6. 图像绘制
    // 绘制任意圆形、方形和文字
    Mat drawing = img.clone();
    circle(drawing, Point(150, 275), 50, Scalar(255, 0, 0), 3);  //绘制蓝色圆
    rectangle(drawing, Point(100, 100), Point(200, 200), Scalar(0, 255, 0), 2); //绘制绿色正方形    
    putText(drawing, "Genshin Impact", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);   //绘制红色额......(doge)
    imwrite("../resources/drawing_shapes.png", drawing);

    // 7. 图像旋转
    Point2f center(img.cols / 2.0, img.rows / 2.0);  //绕几何中心旋转
    Mat rotationMat = getRotationMatrix2D(center, 35, 1.0); //构造旋转矩阵
    Mat rotated;
    warpAffine(img, rotated, rotationMat, img.size());  //旋转并保持旋转后图片大小与原图片相同
    imwrite("../resources/rotated_image.png", rotated);

    // 8. 图像裁剪
    Rect roi(0, 0, img.cols / 2, img.rows / 2); //定义矩形裁减区域
    Mat cropped = img(roi); //进行裁剪
    imwrite("../resources/cropped_image.png", cropped);

    return 0;
}
