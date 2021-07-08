#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

using namespace std;
using namespace cv;

/*
argv:
1 - input - depth folder path
2 - input - filename
3 - output - noise depth folder path
4 - output - noise depth extension
5 - sigma
*/

int main(int argc, char **argv)
{

    std::string filename = argv[1];
    const int divider = atoi(argv[2]);
    std::cout << filename << std::endl;
    cv::Mat mat_depth = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);    
    printf("%u/100=", mat_depth.at<uint16_t>(100, 100));
    cv::Mat output = cv::Mat::zeros(mat_depth.rows, mat_depth.cols, CV_16UC1);
    output = mat_depth / divider;
    printf("%u\n", output.at<uint16_t>(100, 100));
    cv::imwrite(filename, output);

    return (0);
}
