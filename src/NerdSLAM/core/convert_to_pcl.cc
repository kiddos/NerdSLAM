#include "NerdSLAM/core/convert_to_pcl.h"

namespace nerd {
namespace slam {

void Convert(const PointCloud& pc, pcl::PointCloud<pcl::PointXYZRGB>& cloud) {
  int size = pc.points_size();
  for (int i = 0; i < size; ++i) {
    pcl::PointXYZRGB p = {
      static_cast<uint8_t>(pc.points(i).r()),
      static_cast<uint8_t>(pc.points(i).g()),
      static_cast<uint8_t>(pc.points(i).b())
    };
    p.x = pc.points(i).x();
    p.y = pc.points(i).y();
    p.z = pc.points(i).z();
    cloud.points.push_back(p);
  }
}

} /* end of slam namespace */
} /* end of nerd namespace */
