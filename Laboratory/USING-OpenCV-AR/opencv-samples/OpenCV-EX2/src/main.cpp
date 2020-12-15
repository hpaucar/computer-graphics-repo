
// opencv headers
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

// basic headers
#include <iostream>
#include <cstdlib>

/************************* dictionary:
 * DICT_4X4_50=0
 * DICT_4X4_100=1
 * DICT_4X4_250=2
 * DICT_4X4_1000=3
 * DICT_5X5_50=4
 * DICT_5X5_100=5
 * DICT_5X5_250=6
 * DICT_5X5_1000=7
 * DICT_6X6_50=8
 * DICT_6X6_100=9
 * DICT_6X6_250=10
 * DICT_6X6_1000=11
 * DICT_7X7_50=12
 * DICT_7X7_100=13
 * DICT_7X7_250=14
 * DICT_7X7_1000=15
 * DICT_ARUCO_ORIGINAL = 16
 *************************/

int main(int argc, char **argv)
{
    int wait_time = 10;
    int dict_number = 16; // DICT_ARUCO_ORIGINAL
    int id_camera = 0; // 0 = camera default, 1 = second camera
    cv::VideoCapture in_video;
    in_video.open(id_camera);

    if (!in_video.isOpened()) {
        std::cerr << "failed to open camera (id=" << id_camera<<")." << std::endl;
        return 1;
    }

    cv::Ptr<cv::aruco::Dictionary> dictionary;
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(dict_number));

    while (in_video.grab()) {
        cv::Mat image, image_copy;
        in_video.retrieve(image);
        image.copyTo(image_copy);
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners;
        cv::aruco::detectMarkers(image, dictionary, corners, ids);

        // If at least one marker detected
        if (ids.size() > 0)
            cv::aruco::drawDetectedMarkers(image_copy, corners, ids);

        imshow("Aruco Marker Detected", image_copy);
        char key = (char)cv::waitKey(wait_time);
        if (key == 27) // ascii 27 = ESC
            break;
    }

    in_video.release();

    return 0;
}
