#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <fstream>
#include <string>

bool check_ext(const std::string &fn, const std::string &ext)
{
  return fn.substr(fn.find_last_of(".") + 1) == ext;
}

std::string replace_ext(const std::string &fn, const std::string &ext)
{
  return fn.substr(0, fn.find_last_of(".")) + "." + ext;
}

void obj2pcd(const std::string& inputFilename, const std::string& outputFilename)
{
    pcl::PointCloud<pcl::PointXYZ> cloud;

    // Input stream
    std::ifstream is(inputFilename.c_str());

    // Read line by line
    for(std::string line; std::getline(is, line); )
    {
        std::istringstream in(line);

        std::string v;
        in >> v;
        if (v != "v") continue;

        // Read x y z
        float x, y, z;
        in >> x >> y >> z;
        cloud.push_back(pcl::PointXYZ(x, y, z));
    }

    is.close();

    // Save to pcd file
    pcl::io::savePCDFileBinaryCompressed(outputFilename, cloud);
}

int main(int argc, char** argv)
{
  std::string srcfilename = argv[1];
  std::string dstfilename;
  if (check_ext(srcfilename, "obj"))
  {
    if (dstfilename.empty())
      dstfilename = replace_ext(srcfilename, "pcd");
    std::cout << "Converting " << srcfilename << " -> " << dstfilename << std::endl;
    obj2pcd(srcfilename, dstfilename);
    std::cout << "Done." << std::endl;
  } else {
    std::cerr << "Error: The file extension must be .obj" << std::endl;
    return 1;
  }

  return 0;
}