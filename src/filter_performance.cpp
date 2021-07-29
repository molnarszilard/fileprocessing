#include <boost/algorithm/string.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <pcl/console/parse.h>
#include <pcl/console/print.h>
#include <pcl/console/time.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/search/kdtree.h>
#include <string>

using namespace std;
using namespace pcl;
using namespace pcl::io;
using namespace pcl::console;
using namespace pcl::search;

typedef PointXYZ PointType;
typedef PointCloud<PointXYZ> Cloud;

float compare(int cnt, std::string gtdir, std::string preddir, std::string gtending, std::string predending, double K[], int height, int width)
{
    Cloud::Ptr cloudgt(new Cloud);
    Cloud::Ptr cloudpred(new Cloud);

    char file_gt[200];
    char file_pred[200];
    char file_delta[200];

    sprintf(file_gt, "%s%05d%s", gtdir.c_str(), cnt, gtending.c_str());
    sprintf(file_pred, "%s%05d%s", preddir.c_str(), cnt, predending.c_str());

    if (pcl::io::loadPCDFile<pcl::PointXYZ>(file_gt, *cloudgt) == -1) //* load the file
    {
        PCL_ERROR("Couldn't read file \n");
        return (-1);
    }

    if (pcl::io::loadPCDFile<pcl::PointXYZ>(file_pred, *cloudpred) == -1) //* load the file
    {
        PCL_ERROR("Couldn't read file \n");
        return (-1);
    }

    std::cout << "PCD_GT " << cnt << " has: " << cloudgt->size() << " data points." << std::endl; //*
    std::cout << "PCD_PRED has: " << cloudpred->size() << " data points." << std::endl;           //*
    TicToc tt;
    tt.tic();
    double fx = K[0];
    double fy = K[4];
    double x0 = K[2];
    double y0 = K[5];

    int pixel_pos_x, pixel_pos_y;
    double z, u, v;

    double ***arraygt = new double **[height];
    double ***arraypred = new double **[height];

    for (int i = 0; i < height; i++)
    {
        arraygt[i] = new double *[width];
        arraypred[i] = new double *[width];
        for (int j = 0; j < width; j++)
        {
            arraygt[i][j] = new double[3];
            arraypred[i][j] = new double[3];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < 3; k++)
            {
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
            arraygt[pixel_pos_y][pixel_pos_x][0] = cloudgt->points[i].x;
            arraygt[pixel_pos_y][pixel_pos_x][1] = cloudgt->points[i].y;
            arraygt[pixel_pos_y][pixel_pos_x][2] = cloudgt->points[i].z;
        }
    }
    for (int i = 0; i < cloudpred->points.size(); i++)
    {
        bool nan = false;
        if (isnan(cloudpred->points[i].x) || isnan(cloudpred->points[i].y) || isnan(cloudpred->points[i].z))
            nan = true;
        if (isinf(cloudpred->points[i].x) || isinf(cloudpred->points[i].y) || isinf(cloudpred->points[i].z))
            nan = true;
        if (cloudpred->points[i].z == 0.0)
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
            arraypred[pixel_pos_y][pixel_pos_x][0] = cloudpred->points[i].x;
            arraypred[pixel_pos_y][pixel_pos_x][1] = cloudpred->points[i].y;
            arraypred[pixel_pos_y][pixel_pos_x][2] = cloudpred->points[i].z;
        }
    }
    double depthloss = 0, lossgt = 0, losspred = 0;
    int gtonly = 0, predonly = 0, both = 0, no = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (arraygt[i][j][2] > 0.0 && arraypred[i][j][2] > 0.0)
            {
                depthloss += pow((arraygt[i][j][2] * 1000 - arraypred[i][j][2] * 1000), 2);
                both++;
            }
            else
                no++;
            if (arraygt[i][j][2] == 0.0 && arraypred[i][j][2] > 0.0)
            {
                losspred += pow(arraypred[i][j][2] * 1000, 2);
                predonly++;
            }
            if (arraygt[i][j][2] > 0.0 && arraypred[i][j][2] == 0.0)
            {
                lossgt += pow(arraygt[i][j][2] * 1000, 2);
                gtonly++;
            }
        }
    }
    printf("None: %d\n", no);
    depthloss /= both;
    losspred /= predonly;
    lossgt /= gtonly;
    pcl::search::KdTree<PointType> treepred;
    treepred.setInputCloud(cloudpred->makeShared());
    float max_dist_a = 0.0;
    float hausdorf_dist_a = 0.0;
    for (size_t i = 0; i < cloudgt->points.size(); ++i)
    {
        std::vector<int> indices(1);
        std::vector<float> sqr_distances(1);

        treepred.nearestKSearch(cloudgt->points[i], 1, indices, sqr_distances);
        max_dist_a = max_dist_a + sqr_distances[0];
    }
    max_dist_a /= cloudgt->points.size();
    hausdorf_dist_a = std::sqrt(max_dist_a);

    // compare B to A
    pcl::search::KdTree<PointType> treegt;
    treegt.setInputCloud(cloudgt->makeShared());
    float max_dist_b = 0;
    float hausdorf_dist_b = 0;
    for (size_t i = 0; i < cloudpred->points.size(); ++i)
    {
        std::vector<int> indices(1);
        std::vector<float> sqr_distances(1);

        treegt.nearestKSearch(cloudpred->points[i], 1, indices, sqr_distances);
        max_dist_b = max_dist_b + sqr_distances[0];
    }
    max_dist_b /= cloudpred->points.size();
    hausdorf_dist_b = std::sqrt(max_dist_b);

    float dist = std::max(hausdorf_dist_a, hausdorf_dist_b);
    float chamfer = max_dist_b + max_dist_a;
    print_info("[done, ");
    print_value("%g", tt.toc());
    print_info(" ms : ");
    print_info("A->B: ");
    print_value("%f", hausdorf_dist_a);
    print_info(", B->A: ");
    print_value("%f", hausdorf_dist_b);
    print_info(", Hausdorff Distance: ");
    print_value("%f", dist);
    print_info(", Chamfer Distance: ");
    print_value("%f", chamfer);
    print_info(" ]\n");
    printf("Depth distance: %f, number: %d", depthloss, both);
    printf("\nGT only distance: %f, number: %d", lossgt, gtonly);
    printf("\nPred only distance: %f, number: %d\n", losspred, predonly);
    return chamfer;
}

int main(int argc, char *argv[])
{
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
    std::string histoname = argv[1];
    std::string gtdir = argv[2];
    std::string preddir = argv[3];
    std::string gtending = argv[4];
    std::string predending = argv[5];

    int start = atoi(argv[6]);
    int end = atoi(argv[7]);
    int step = atoi(argv[8]);
    int count = start;
    int cnt = 0;
    float distance = 0;
    ofstream myfile;
    myfile.open(histoname);

    while (count < end)
    {
        float q = compare(count, gtdir, preddir, gtending, predending, K, height, width);
        // printf("%f\n",q);
        myfile << q << "\n";
        distance = distance + q;
        count = count + step;
        cnt++;
    }
    double D = distance / cnt;
    printf("The TOTAL distance for %d pointclouds is: %f\n\n", cnt, D);
    myfile.close();
    return 0;
}
