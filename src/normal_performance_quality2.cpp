#include <boost/algorithm/string.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <string>

using namespace std;

double compare(int cnt, std::string gtdir, std::string preddir, std::string gtending, std::string predending, double K[], int height, int width)
{

    pcl::PointCloud<pcl::PointNormal>::Ptr cloudgt(new pcl::PointCloud<pcl::PointNormal>);
    pcl::PointCloud<pcl::PointNormal>::Ptr cloudpred(new pcl::PointCloud<pcl::PointNormal>);

    char file_gt[200];
    char file_pred[200];
    sprintf(file_gt, "%s%05d%s", gtdir.c_str(), cnt, gtending.c_str());
    sprintf(file_pred, "%s%05d%s", preddir.c_str(), cnt, predending.c_str());

    if (pcl::io::loadPCDFile<pcl::PointNormal>(file_gt, *cloudgt) == -1) //* load the file
    {
        PCL_ERROR("Couldn't read file \n");
        return (-1);
    }

    if (pcl::io::loadPCDFile<pcl::PointNormal>(file_pred, *cloudpred) == -1) //* load the file
    {
        PCL_ERROR("Couldn't read file \n");
        return (-1);
    }

    double fx = K[0];
    double fy = K[4];
    double x0 = K[2];
    double y0 = K[5];

    int pixel_pos_x, pixel_pos_y;
    double z, u, v;

    double*** arraygt = new double**[height];
    double*** arraypred = new double**[height];
 
    for (int i = 0; i < height; i++) {
        arraygt[i] = new double*[width]; 
        arraypred[i] = new double*[width];
        for (int j = 0; j < width; j++) {
            arraygt[i][j] = new double[4];
            arraypred[i][j] = new double[4];
        }
    }
 
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 4; k++) {
                arraygt[i][j][k] = 0;
                arraypred[i][j][k] = 0;
            }
        }
    }

    for (int i = 0; i < cloudgt->points.size(); i++)
    {
        bool nan = false;
        if (isnan(cloudgt->points[i].x) || isnan(cloudgt->points[i].y) || isnan(cloudgt->points[i].z))
            nan = true;
        if (isinf(cloudgt->points[i].x) || isinf(cloudgt->points[i].y) || isinf(cloudgt->points[i].z))
            nan = true;
        if (cloudgt->points[i].z < 1 / 10000)
            nan = true;
        if (!nan)
        {
            z = cloudgt->points[i].z * 1000.0;
            u = (cloudgt->points[i].x * 1000.0 * fx) / z;
            v = (cloudgt->points[i].y * 1000.0 * fy) / z;
            pixel_pos_x = round(u + x0);
            pixel_pos_y = round(v + y0);

            if (pixel_pos_x < 0)
            {
                pixel_pos_x = -pixel_pos_x;
            }
            if (pixel_pos_x > (width - 1))
            {
                pixel_pos_x = width - 1;
            }

            if (pixel_pos_y < 0)
            {
                pixel_pos_y = -pixel_pos_y;
            }
            if (pixel_pos_y > (height - 1))
            {
                pixel_pos_y = height - 1;
            }
            arraygt[pixel_pos_y][pixel_pos_x][0] = z;
            arraygt[pixel_pos_y][pixel_pos_x][1] = cloudgt->points[i].normal_x;
            arraygt[pixel_pos_y][pixel_pos_x][2] = cloudgt->points[i].normal_y;
            arraygt[pixel_pos_y][pixel_pos_x][3] = cloudgt->points[i].normal_z;
        }
    }

    for (int i = 0; i < cloudpred->points.size(); i++)
    {
        bool nan = false;
        if (isnan(cloudpred->points[i].x) || isnan(cloudpred->points[i].y) || isnan(cloudpred->points[i].z))
            nan = true;
        if (isinf(cloudpred->points[i].x) || isinf(cloudpred->points[i].y) || isinf(cloudpred->points[i].z))
            nan = true;
        if (cloudpred->points[i].z < 1 / 10000)
            nan = true;
        if (!nan)
        {
            z = cloudpred->points[i].z * 1000.0;
            u = (cloudpred->points[i].x * 1000.0 * fx) / z;
            v = (cloudpred->points[i].y * 1000.0 * fy) / z;
            pixel_pos_x = round(u + x0);
            pixel_pos_y = round(v + y0);

            if (pixel_pos_x < 0)
            {
                pixel_pos_x = -pixel_pos_x;
            }
            if (pixel_pos_x > (width - 1))
            {
                pixel_pos_x = width - 1;
            }

            if (pixel_pos_y < 0)
            {
                pixel_pos_y = -pixel_pos_y;
            }
            if (pixel_pos_y > (height - 1))
            {
                pixel_pos_y = height - 1;
            }
            arraypred[pixel_pos_y][pixel_pos_x][0] = z;
            arraypred[pixel_pos_y][pixel_pos_x][1] = cloudpred->points[i].normal_x;
            arraypred[pixel_pos_y][pixel_pos_x][2] = cloudpred->points[i].normal_y;
            arraypred[pixel_pos_y][pixel_pos_x][3] = cloudpred->points[i].normal_z;
        }
    }
    std::cout << "PCD_GT has: " << cloudgt->size() << " data points." << std::endl;     //*
    std::cout << "PCD_PRED has: " << cloudpred->size() << " data points." << std::endl; //*
    double quality;
    double cloud_quality = 0;
    int histo[100] = {0};
    int counter = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (arraygt[i][j][0] > 0 && arraypred[i][j][0] > 0)
            {
                counter++;
                Eigen::Vector3f normalgt;
                normalgt << arraygt[i][j][1], arraygt[i][j][2], arraygt[i][j][3];
                normalgt.normalize();
                Eigen::Vector3f normalpred;
                normalpred << arraypred[i][j][1], arraypred[i][j][2], arraypred[i][j][3];
                normalpred.normalize();
                quality = normalgt.dot(normalpred);
                double angle = acos(quality);
                bool nan = false;
                if (quality != quality || std::isnan(quality))
                    nan = true;
                if (!nan)
                {
                    if (quality < 0)
                    {
                        quality = -quality;
                    }
                    cloud_quality = cloud_quality + quality;
                }
            }
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            delete[] arraygt[i][j];
            delete[] arraypred[i][j];
        }
        delete[] arraygt[i];
        delete[] arraypred[i];
    }
    delete[] arraygt;
    delete[] arraypred;
    
    cloud_quality = cloud_quality / counter;
    return cloud_quality;
}

int main(int argc, char *argv[])
{

    std::string histoname = argv[1];
    std::string gtdir = argv[2];
    std::string preddir = argv[3];
    std::string gtending = argv[4];
    std::string predending = argv[5];
    int start = atoi(argv[6]);
    int end = atoi(argv[7]);
    int step = atoi(argv[8]);
    int height = 480;
    int width = 640;
    std::string camera_type = argv[9];
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
        height = 360;
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

    int count = start;
    int cnt = 0;
    double distance = 0;
    ofstream myfile;
    myfile.open(histoname);
    double Quality = 0;
    int histogram[100] = {};

    while (count < end)
    {
        double q = compare(count, gtdir, preddir, gtending, predending, K, height, width);
        printf("%f\n", q);
        int ind = round(q * 100);
        if (ind >= 100)
            ind = 99;
        histogram[ind]++;
        printf("quality for %05d pcd is: %f\n", count, q);
        Quality += q;
        count = count + step;
        cnt++;
    }
    double Q = Quality / cnt;
    printf("The TOTAL quality for %d pointclouds is: %f\n\n", cnt, Q);
    printf("The histogram:\n");
    for (int i = 0; i < 100; i++)
    {
        if (histogram[i] > 0)
        {
            myfile << i << " " << histogram[i] << "\n";
            printf("%02d:%04d - ", i, histogram[i]);
            for (int j = 0; j < histogram[i]; j++)
                printf("|");
            printf("\n");
        }
    }
    myfile.close();
    return 0;
}
