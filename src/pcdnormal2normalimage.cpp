#include <boost/algorithm/string.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pcl/common/eigen.h>
#include <pcl/features/integral_image_normal.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/search/impl/search.hpp>
#include <pcl/visualization/pcl_visualizer.h>
#include <string>
#include <thread>

using namespace cv;
using namespace std;

int min_points = 0;

int main(int argc, char **argv)
{
    int height = 480;
    int width = 640;
    std::string camera_type = argv[4];
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

    std::string ddir = argv[1];
    std::string normaldir = argv[2];
    std::string filename = argv[3];
    int height_ = 480;
    int width_ = 640;

    char file_in[200];
    char file_out[200];

    sprintf(file_in, "%s%s", ddir.c_str(), filename.c_str());
    filename = filename.substr(0, filename.size() - 3);
    sprintf(file_out, "%s%spng", normaldir.c_str(), filename.c_str());
    printf("Processing file - %s\n", file_in);

    pcl::PointCloud<pcl::PointNormal>::Ptr cloud(new pcl::PointCloud<pcl::PointNormal>);
    if (pcl::io::loadPCDFile<pcl::PointNormal>(file_in, *cloud) == -1) //* load the file
    {
        PCL_ERROR("Couldn't read file \n");
        return (-1);
    }
    std::cout << "PointCloud has: " << cloud->size() << " data points." << std::endl; //*

    cv::Mat output = cv::Mat::zeros(height_, width_, CV_8UC3);
    pcl::PointNormal pn;
    bool nan = false;
    int pixel_pos_x, pixel_pos_y;
    float z, u, v;
    for (int i = 0; i < cloud->size(); i++)
    {
        pn.x = cloud->points[i].x;
        pn.y = cloud->points[i].y;
        pn.z = cloud->points[i].z;
        pn.normal_x = cloud->points[i].normal_x;
        pn.normal_y = cloud->points[i].normal_y;
        pn.normal_z = cloud->points[i].normal_z;
        if (pn.x != pn.x || pn.y != pn.y || pn.z != pn.z || std::isnan(pn.x) || std::isnan(pn.y) || std::isnan(pn.z))
            nan = true;
        if (!nan)
        {

            uint8_t r, g, b;
            double r0 = cloud->points[i].normal_x;
            double g0 = cloud->points[i].normal_y;
            double b0 = cloud->points[i].normal_z;
            Eigen::Vector3f vector;
            vector[0] = r0;
            vector[1] = g0;
            vector[2] = b0;
            vector.normalize();
            vector[0] = (vector[0] + 1) / 2*255;
            vector[1] = (vector[1] + 1) / 2*255;
            vector[2] = (vector[2] + 1) / 2*255;
            
            r = round(vector[0]);
            g = round(vector[1]);
            b = round(vector[2]);
            z = cloud->points[i].z * 1000.0;
            u = (cloud->points[i].x * 1000.0 * fx) / z;
            v = (cloud->points[i].y * 1000.0 * fy) / z;
            pixel_pos_x = round(u + x0);
            pixel_pos_y = round(v + y0);

            if (pixel_pos_x < 0)
            {
                pixel_pos_x = -pixel_pos_x;
            }
            if (pixel_pos_x > (width_ - 1))
            {
                pixel_pos_x = width_ - 1;
            }

            if (pixel_pos_y < 0)
            {
                pixel_pos_y = -pixel_pos_y;
            }
            if (pixel_pos_y > (height_ - 1))
            {
                pixel_pos_y = height_ - 1;
            }
            output.at<Vec3b>(pixel_pos_y, pixel_pos_x)[0] = b;
            output.at<Vec3b>(pixel_pos_y, pixel_pos_x)[1] = g;
            output.at<Vec3b>(pixel_pos_y, pixel_pos_x)[2] = r;
        }
        nan = false;
    }
    imwrite(file_out, output);
    return 0;
}
