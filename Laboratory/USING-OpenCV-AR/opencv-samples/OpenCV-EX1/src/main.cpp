#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
    cv::Mat img;
    img = cv::imread("test.jpg");
    cv::imshow("my first opencv window", img);
    cv::waitKey();
    return 0;
}
