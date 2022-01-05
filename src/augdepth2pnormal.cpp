#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <string>

using namespace std;

char directory[200] = "/media/rambo/ssd2/Szilard/c24/640x480/";
int cnt = 0;
int counter = 0;
int counter2 = 0;
int max_nr = 700;

void depth_normals_to_pcdnormals()
{
    int height = 480;
    int width = 640;
    std::string camera_type = "isaac";
    std::string camera_type_pico = "pico";
    std::string camera_type_nyu = "nyu";
    std::string camera_type_kitti = "kitti";
    std::string camera_type_isaac = "isaac";
    double K[9] = {582.62448167737955, 0.0, 313.04475870804731, 0.0, 582.69103270988637, 238.44389626620386, 0.0, 0.0, 1.0}; // nyu_v2_dataset
    if (camera_type.compare(camera_type_pico) == 0)
    {
        height = 360;
        width = 640;
        K[0] = 460.58518931365654;
        K[2] = 334.0805877590529;
        K[4] = 460.2679961517268;
        K[5] = 169.80766383231037; // pico zense
    }
    if (camera_type.compare(camera_type_nyu) == 0)
    {
        height = 480;
        width = 640;
        K[0] = 582.62448167737955;
        K[2] = 313.04475870804731;
        K[4] = 582.69103270988637;
        K[5] = 238.44389626620386; // nyu v2
    }
    if (camera_type.compare(camera_type_kitti) == 0)
    {
        height = 350;
        width = 1200;
        K[0] = 721.5377;
        K[2] = 609.5593;
        K[4] = 721.5377;
        K[5] = 149.854; // kitti - average
    }

    if (camera_type.compare(camera_type_isaac) == 0)
    {
        height = 480;
        width = 640;
        K[0] = 581.8181762695312;
        K[2] = 320.0;
        K[4] = 581.8181762695312;
        K[5] = 240.0; //isaac
    }

    double fx = K[0];
    double fy = K[4];
    double x0 = K[2];
    double y0 = K[5];
    char file_din[200];
    char file_dinflr[200];
    char file_dinfud[200];
    char file_normal[200];

    pcl::PointCloud<pcl::PointNormal>::Ptr cloud(new pcl::PointCloud<pcl::PointNormal>);
    pcl::PointCloud<pcl::PointNormal> cloud1;
    pcl::PointNormal p1;
    pcl::PointCloud<pcl::PointNormal> cloud2;
    pcl::PointNormal p2;
    pcl::PointCloud<pcl::PointNormal> cloud3;
    pcl::PointNormal p3;
    printf("Converting file: %saug/depth/%05d.png\n", directory, counter);
    sprintf(file_din, "%saug/depth/%05d.png", directory, counter);
    sprintf(file_dinflr, "%saug/depth/%05d.png", directory, counter + 1);
    sprintf(file_dinfud, "%saug/depth/%05d.png", directory, counter + 2);
    // sprintf(file_normal, "%snormalimages/%05d.png", directory, cnt);
    sprintf(file_normal, "%spcdnormals/%05d_normal.pcd", directory, cnt);
    // sprintf(file_d3out1, "%sadepth3/%05d_depth3.png", directory, counter2);
    // sprintf(file_d3out2, "%sadepth3/%05d_depth3.png", directory, counter2 + 1);
    // sprintf(file_d3out3, "%sadepth3/%05d_depth3.png", directory, counter2 + 2);
    // sprintf(file_depthout1, "%saug/adepth/%05d.png", directory, counter2);
    // sprintf(file_depthout2, "%saug/adepth/%05d.png", directory, counter2 + 1);
    // sprintf(file_depthout3, "%saug/adepth/%05d.png", directory, counter2 + 2);
    cv::Mat depth1 = cv::imread(file_din, cv::IMREAD_UNCHANGED);
    cv::Mat depth2 = cv::imread(file_dinflr, cv::IMREAD_UNCHANGED);
    cv::Mat depth3 = cv::imread(file_dinfud, cv::IMREAD_UNCHANGED);

    // cv::imwrite(file_depthout1, depth1);
    // cv::imwrite(file_d3out1, depth31);
    // cv::imwrite(file_depthout2, depth2);
    // cv::imwrite(file_d3out2, depth32);
    // cv::imwrite(file_depthout3, depth3);
    // cv::imwrite(file_d3out3, depth33);

    // ifstream normal_file(file_normal);
    // //Check that the input file has being successfully opened
    // if (!(normal_file.is_open()))
    // {
    //     cout << "[x] Cannot open normal file!" << endl;
    // }
    // cv::Mat normal = cv::imread(file_normal, cv::IMREAD_COLOR);
    if (pcl::io::loadPCDFile<pcl::PointNormal>(file_normal, *cloud) == -1) //* load the file
    {
        PCL_ERROR("Couldn't read file \n");
        return;
    }

    int i1 = 0;
    int j1 = 0;
    int i2 = 0;
    int j2 = depth2.cols - 1;
    int i3 = depth3.rows - 1;
    int j3 = 0;
    for (int i = 0; i < depth1.rows; i++)
    {
        for (int j = 0; j < depth1.cols; j++)
        {
            double d1 = (double)depth1.at<uint16_t>(i1, j1) / 1000.0;
            // double normal_x = (double)(normal.ptr(i, j)[2] * 2.0 / 255.0 - 1.0);
            // double normal_y = (double)(normal.ptr(i, j)[1] * 2.0 / 255.0 - 1.0);
            // double normal_z = (double)(normal.ptr(i, j)[0] * 2.0 / 255.0 - 1.0);
            double normal_x = cloud->points[i*depth1.cols+j].normal_x;
            double normal_y = cloud->points[i*depth1.cols+j].normal_y;
            double normal_z = cloud->points[i*depth1.cols+j].normal_z;
            if (d1 != 0.0)
            {
                float x_over_z = (j1 - x0) / fx;
                float y_over_z = (i1 - y0) / fy;
                p1.z = d1;
                p1.x = x_over_z * p1.z;
                p1.y = y_over_z * p1.z;
                p1.normal_x = normal_x;
                p1.normal_y = normal_y;
                p1.normal_z = normal_z;
                cloud1.points.push_back(p1);

                x_over_z = (j2 - x0) / fx;
                y_over_z = (i2 - y0) / fy;
                p2.z = d1;
                p2.x = x_over_z * p2.z;
                p2.y = y_over_z * p2.z;
                p2.normal_x = -normal_x;
                p2.normal_y = normal_y;
                p2.normal_z = normal_z;
                cloud2.points.push_back(p2);

                x_over_z = (j3 - x0) / fx;
                y_over_z = (i3 - y0) / fy;
                p3.z = d1;
                p3.x = x_over_z * p3.z;
                p3.y = y_over_z * p3.z;
                p3.normal_x = normal_x;
                p3.normal_y = -normal_y;
                p3.normal_z = normal_z;
                cloud3.points.push_back(p3);
            }

            j1++;
            if (j1 >= depth1.cols)
            {
                j1 = 0;
                i1++;
            }
            j2--;
            if (j2 < 0)
            {
                j2 = depth2.cols - 1;
                i2++;
            }
            j3++;
            if (j3 >= depth3.cols)
            {
                j3 = 0;
                i3--;
            }
        }
    }

    cloud1.width = cloud1.points.size();
    cout << "Number of points:" << cloud1.width << endl;
    cloud1.height = 1;
    cloud1.points.resize(cloud1.width * cloud1.height);
    cloud1.is_dense = false;
    char file_pcd1[100];
    sprintf(file_pcd1, "%saug/pcdnormals/%05d.pcd", directory, counter2);
    pcl::io::savePCDFile(file_pcd1, cloud1, true);

    cloud2.width = cloud2.points.size();
    cout << "Number of points:" << cloud2.width << endl;
    cloud2.height = 1;
    cloud2.points.resize(cloud2.width * cloud2.height);
    cloud2.is_dense = false;
    char file_pcd2[100];
    sprintf(file_pcd2, "%saug/pcdnormals/%05d.pcd", directory, counter2 + 1);
    pcl::io::savePCDFile(file_pcd2, cloud2, true);

    cloud3.width = cloud3.points.size();
    cout << "Number of points:" << cloud3.width << endl;
    cloud3.height = 1;
    cloud3.points.resize(cloud3.width * cloud3.height);
    cloud3.is_dense = false;
    char file_pcd3[100];
    sprintf(file_pcd3, "%saug/pcdnormals/%05d.pcd", directory, counter2 + 2);
    pcl::io::savePCDFile(file_pcd3, cloud3, true);

    cout << "[*] Conversion finished!" << endl;
    counter = counter + 3;
    counter2 = counter2 + 3;
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
