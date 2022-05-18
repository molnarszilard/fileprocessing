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
    std::string indir = argv[3];
    std::string infilename = argv[4];

    char file_depth[200];
    sprintf(file_depth, "%s%s", ddir.c_str(), infilename.c_str());
    char file_ir[200];
    sprintf(file_ir, "%s%s", idir.c_str(), infilename.c_str());
    char file_in[200];
    sprintf(file_in, "%s%s", indir.c_str(), infilename.c_str());
    // depth2ir=ch0-depth, ch1-depth, ch2-ir
    // depth2ir=ch0-depth, ch1-ir, ch2-ir
    Mat mat_in = imread(file_in, IMREAD_UNCHANGED);
    int rows = mat_in.rows;
    int cols = mat_in.cols;
    // int i = 0;
    // int j = 0;
    // do
    // {
    //     i += 10;
    //     if (i >= 360)
    //     {
    //         i = 0;
    //         j += 10;
    //     }
    //     if(j>=640) break;
    //     if (mat_in.at<cv::Vec3s>(i, j)[0] > 0)
    //     {
    //         cout << "ch0: " << mat_in.at<cv::Vec3s>(i, j)[0] << endl;
    //         cout << "ch1: " << mat_in.at<cv::Vec3s>(i, j)[1] << endl;
    //         cout << "ch2: " << mat_in.at<cv::Vec3s>(i, j)[2] << endl;
    //     }

    // } while (mat_in.at<cv::Vec3s>(i, j)[0] == 0);

    Mat splitted_mat[3];         // destination array
    split(mat_in, splitted_mat); // split source

    cout << "ch0: " << splitted_mat[0].at<unsigned short>(155, 155) << endl; //depth
    cout << "ch1: " << splitted_mat[1].at<unsigned short>(155, 155) << endl; //depth or ir
    cout << "ch2: " << splitted_mat[2].at<unsigned short>(155, 155) << endl; //ir

    // imwrite(file_depth,splitted_mat[0]);
    // imwrite("green.png",splitted_mat[1]);
    // imwrite(file_ir,splitted_mat[2]);

    return (0);
}
