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

void pcd2obj(const std::string& inputFilename, const std::string& outputFilename)
{
    pcl::PointCloud<pcl::PointXYZ> cloud;

    if (pcl::io::loadPCDFile<pcl::PointXYZ> (inputFilename, cloud) == -1)
    {
        std::cerr << "Couldn't read file " << inputFilename << std::endl;
        return;
    }

    const size_t size = cloud.points.size();
    std::ofstream os(outputFilename.c_str());

    for(unsigned int i=0 ; i<size ; i++)
    {
        // Remove nan
        if(!std::isnan(cloud.points[i].x))
        {
            os << "v ";
            os << cloud.points[i].x << " ";
            os << cloud.points[i].y << " ";
            os << cloud.points[i].z << "\n";
        }
    }

    os.close();
}

int main(int argc, char** argv)
{
  std::string srcfilename = argv[1];
  std::string dstfilename;
  if (check_ext(srcfilename, "pcd"))
  {
    if (dstfilename.empty())
      dstfilename = replace_ext(srcfilename, "pcd");
    std::cout << "Converting " << srcfilename << " -> " << dstfilename << std::endl;
    pcd2obj(srcfilename, dstfilename);
    std::cout << "Done." << std::endl;
  } else {
    std::cerr << "Error: The file extension must be .pcd" << std::endl;
    return 1;
  }

  return 0;
}