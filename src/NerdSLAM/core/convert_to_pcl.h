#ifndef CONVERT_TO_PCL_H
#define CONVERT_TO_PCL_H

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "nerdslam.pb.h"

namespace nerd {
namespace slam {

void Convert(const PointCloud& pc, pcl::PointCloud<pcl::PointXYZRGB>& cloud);

} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: CONVERT_TO_PCL_H */
