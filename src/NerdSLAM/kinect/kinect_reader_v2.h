#ifndef KINECT_V2_READER_H
#define KINECT_V2_READER_H

#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/libfreenect2.hpp>
#include "NerdSLAM/kinect/kinect_reader.h"

namespace nerd {
namespace slam {

class Kinect2Reader : public KinectReader {
 public:
  Kinect2Reader();
  Kinect2Reader(const Kinect2Config& config);
  ~Kinect2Reader();
  virtual bool FindDevice() override;
  virtual void StartDevice() override;
  virtual void PauseDevice() override;
  virtual void StopDevice() override;

 private:
  void Loop();
  void ProcessRGBFrame(const libfreenect2::Frame* rgb);
  void ProcessIRFrame(const libfreenect2::Frame* ir);
  void ProcessDepthFrame(const libfreenect2::Frame* depth);

  Kinect2Config config_;

  libfreenect2::Freenect2 freenect2_;
  libfreenect2::Freenect2Device* device_;
  libfreenect2::PacketPipeline* pipeline_;
  libfreenect2::FrameMap frame_map_;
  libfreenect2::SyncMultiFrameListener* listener_;
};

} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_V2_READER_H */
