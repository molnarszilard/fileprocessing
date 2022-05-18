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
    std::string idir = argv[2];
    std::string outdir = argv[3];
    std::string dfilename = argv[4];
    std::string ifilename = argv[5];

    char file_depth[200];
    sprintf(file_depth, "%s%s", ddir.c_str(), dfilename.c_str());
    char file_ir[200];
    sprintf(file_ir, "%s%s", idir.c_str(), ifilename.c_str());
    char file_out[200];
    sprintf(file_out, "%s%s", outdir.c_str(), dfilename.c_str());

    cv::Mat mat_depth = cv::imread(file_depth, IMREAD_UNCHANGED);
    cv::Mat mat_ir = cv::imread(file_ir, IMREAD_UNCHANGED);
    cv::Mat zerochannel = cv::Mat::zeros(cv::Size(mat_depth.rows, mat_depth.cols), CV_16U);
    cv::Mat output = cv::Mat::zeros(mat_depth.rows, mat_depth.cols, CV_16UC3);
    cv::Mat images[3] = {mat_ir, mat_ir, mat_depth};
    int dims[3] = {2, mat_depth.rows, mat_depth.cols};
    cv::Mat joined(3, dims, CV_16U);
    for (int i = 0; i < 3; i++)
    {
        uint16_t *ptr = &joined.at<uint16_t>(i, 0, 0);                            // pointer to first element of slice i
        cv::Mat destination(mat_depth.rows, mat_depth.cols, CV_32S, (void *)ptr); // no data copy, see documentation
        images[i].copyTo(destination);
    }

    for (int x = 0; x < images[0].rows; x++)
    {
        for (int y = 0; y < images[0].cols; y++)
        {
            output.at<cv::Vec3s>(x, y)[2] = images[0].at<unsigned short>(x, y);
            output.at<cv::Vec3s>(x, y)[1] = images[1].at<unsigned short>(x, y);
            output.at<cv::Vec3s>(x, y)[0] = images[2].at<unsigned short>(x, y);
        }
    }
    cv::imwrite(file_out, output);

    return (0);
}
