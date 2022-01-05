#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
using namespace std;

int main(int argc, char *argv[])
{
    vector<string> result;
    vector<string> numbers;
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointXYZ p;
    std::string idir = argv[1];
    std::string odir = argv[2];
    std::string filename = argv[3];
    char file_in[200];
    sprintf(file_in, "%s%s", idir.c_str(), filename.c_str());
    filename = filename.substr(0, filename.size() - 3);
    char file_out[200];
    sprintf(file_out, "%s%spcd", odir.c_str(), filename.c_str());

    ifstream off_file(file_in);
    if (!(off_file.is_open()))
    {
        cout << "[x] Cannot open off file!" << endl;
    }
    cout << "Processing: " << file_in << endl;
    string content = "";
    float outlier;
    string fl = "";
    string sl = "";
    getline(off_file, fl);
    if (fl.compare("OFF") == 0 || fl.compare("OF") == 0)
    {
        getline(off_file, sl);
    }
    else
    {
        sl = fl;
    }
    boost::split(numbers, sl, [](char c)
                 { return c == ' '; });
    int nr_vertices = stoi(numbers[0]);
    int nr_faces = stoi(numbers[1]);
    int nr_cells = stoi(numbers[2]);
    for (int i = 0; i < nr_vertices; i++)
    {
        getline(off_file, content);
        boost::split(result, content, [](char c)
                     { return c == ' '; });
        p.x = stof(result[0]);
        p.y = stof(result[1]);
        p.z = stof(result[2]);
        cloud.points.push_back(p);
    }

    cloud.width = cloud.points.size();
    cout << "Number of points:" << cloud.width << endl;
    cloud.height = 1;
    cloud.points.resize(cloud.width * cloud.height);
    cloud.is_dense = false;
    off_file.close();
    pcl::io::savePCDFile(file_out, cloud, true);
    cout << "[*] Conversion finished!" << endl;

    return 0;
}
