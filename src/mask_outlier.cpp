#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    std::string ddir = argv[1];
    std::string mdir = argv[2];
    std::string dfilename = argv[3];
    int multiplier = atof(argv[4]);
    char file_depth[200];
    char file_mask[200];
    int cnt = 0;

    sprintf(file_depth, "%s%s", ddir.c_str(), dfilename.c_str());
    dfilename = dfilename.substr(0, dfilename.size() - 4);
    sprintf(file_mask, "%s%s_mask.png", mdir.c_str(), dfilename.c_str());
    printf("Processing file - %s\n", file_depth);
    cv::Mat mat_depth = cv::imread(file_depth, CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat output = cv::Mat::zeros(mat_depth.rows, mat_depth.cols, CV_8UC1);

    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;

    minMaxLoc(mat_depth, &minVal, &maxVal, &minLoc, &maxLoc);
    for (int x = 0; x < mat_depth.rows; x++)
    {
        for (int y = 0; y < mat_depth.cols; y++)
        {
            if (mat_depth.at<uint8_t>(x, y) > maxVal * multiplier)
            {
                output.at<uint8_t>(x, y) = 255;
            }
            else
            {
                output.at<uint8_t>(x, y) = 0;
            }
        }
    }
    cv::imwrite(file_mask, output);

    return (0);
}
