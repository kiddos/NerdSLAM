#ifndef KINECT_READER_V1_H
#define KINECT_READER_V1_H

#include <libfreenect.hpp>
#include "NerdSLAM/kinect/kinect_reader.h"
#include "NerdSLAM/kinect/v1/kinect_device.h"
#include "nerdslam.pb.h"

namespace nerd {
namespace slam {
namespace v1 {

class KinectReaderV1 : public KinectReader {
 public:
  enum { FRAME_WIDTH = 640, FRAME_HEIGHT = 480, FRAME_CHANNEL = 3 };
  KinectReaderV1(const KinectConfigV1& config);
  virtual ~KinectReaderV1();
  bool FindDevice() override;
  void Run() override;
  void StartDevice() override;
  void PauseDevice() override;
  void StopDevice() override;

 private:
  void Loop();

  KinectConfigV1 config_;
  Freenect::Freenect freenect_;
  KinectDevice* device_;
  bool running_, pause_;

  FrameMap frames_;
  std::vector<uint8_t> rgb_, depth_;
};

} /* end of v1 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_READER_V1_H */
