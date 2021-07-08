#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

using namespace std;
using namespace cv;

// char directory[100] = "/media/rambo/ssd2/Szilard/c24/640x480/";
// char directory[100] = "/media/rambo/ssd2/Szilard/lenssen_tofnest/";
char directory[100] = "/media/rambo/ssd2/Szilard/nyu_tofnest/";
int cnt = 0;
int counter = 0;
int max_nr = 1449;

void depth_normals_to_pcdnormals()
{
    char file_normal[100];

    printf("Converting file: %snormalimages/%05d.png\n", directory, cnt);
    sprintf(file_normal, "%snormalimages/%05d.png", directory, cnt);

    cv::Mat normal = cv::imread(file_normal, cv::IMREAD_COLOR);
    cv::Mat output1 = cv::Mat::zeros(normal.rows, normal.cols, CV_8UC3);
    cv::Mat output2 = cv::Mat::zeros(normal.rows, normal.cols, CV_8UC3);
    cv::Mat output3 = cv::Mat::zeros(normal.rows, normal.cols, CV_8UC3);
    for (int i = 0; i < normal.rows; i++)
    {
        for (int j = 0; j < normal.cols; j++)
        {
            output1.at<Vec3b>(i, j)[2] = normal.ptr(i, j)[2];
            output1.at<Vec3b>(i, j)[1] = normal.ptr(i, j)[1];
            output1.at<Vec3b>(i, j)[0] = normal.ptr(i, j)[0];
            if (normal.ptr(i, j)[2] == 0 && normal.ptr(i, j)[1] == 0 && normal.ptr(i, j)[0] == 0)
            {
                double normal_x = (double)(normal.ptr(i, j)[2] * 2.0 / 255.0 - 1.0);
                double normal_y = (double)(normal.ptr(i, j)[1] * 2.0 / 255.0 - 1.0);
                double normal_z = (double)(normal.ptr(i, j)[0] * 2.0 / 255.0 - 1.0);
                uint8_t r, g, b;

                r = round((normal_x + 1) / 2 * 255);
                g = round((normal_y + 1) / 2 * 255);
                b = round((normal_z + 1) / 2 * 255);
                output2.at<Vec3b>(i, normal.cols - j - 1)[0] = b;
                output2.at<Vec3b>(i, normal.cols - j - 1)[1] = g;
                output2.at<Vec3b>(i, normal.cols - j - 1)[2] = r;

                r = round((normal_x + 1) / 2 * 255);
                g = round((normal_y + 1) / 2 * 255);
                b = round((normal_z + 1) / 2 * 255);
                output3.at<Vec3b>(normal.rows - i - 1, j)[0] = b;
                output3.at<Vec3b>(normal.rows - i - 1, j)[1] = g;
                output3.at<Vec3b>(normal.rows - i - 1, j)[2] = r;
            }
            else
            {

                double normal_x = (double)(normal.ptr(i, j)[2] * 2.0 / 255.0 - 1.0);
                double normal_y = (double)(normal.ptr(i, j)[1] * 2.0 / 255.0 - 1.0);
                double normal_z = (double)(normal.ptr(i, j)[0] * 2.0 / 255.0 - 1.0);
                uint8_t r, g, b;

                r = round((-normal_x + 1) / 2 * 255);
                g = round((normal_y + 1) / 2 * 255);
                b = round((normal_z + 1) / 2 * 255);
                output2.at<Vec3b>(i, normal.cols - j - 1)[0] = b;
                output2.at<Vec3b>(i, normal.cols - j - 1)[1] = g;
                output2.at<Vec3b>(i, normal.cols - j - 1)[2] = r;

                r = round((normal_x + 1) / 2 * 255);
                g = round((-normal_y + 1) / 2 * 255);
                b = round((normal_z + 1) / 2 * 255);
                output3.at<Vec3b>(normal.rows - i - 1, j)[0] = b;
                output3.at<Vec3b>(normal.rows - i - 1, j)[1] = g;
                output3.at<Vec3b>(normal.rows - i - 1, j)[2] = r;
            }
        }
    }
    char file_out1[200];
    char file_out2[200];
    char file_out3[200];
    sprintf(file_out1, "%saug/rgb/%05d.png", directory, counter);
    sprintf(file_out2, "%saug/rgb/%05d.png", directory, counter + 1);
    sprintf(file_out3, "%saug/rgb/%05d.png", directory, counter + 2);
    imwrite(file_out1, output1);
    imwrite(file_out2, output2);
    imwrite(file_out3, output3);

    cout << "[*] Conversion finished!" << endl;
    counter = counter + 3;
    cnt++;
}

int main(int argc, char *argv[])
{
    while (cnt < max_nr)
    {
        depth_normals_to_pcdnormals();
    }
    return 0;
}
