#ifndef KINECT_READER_V2_H
#define KINECT_READER_V2_H

#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/logger.h>
#include "NerdSLAM/kinect/kinect_reader.h"

namespace nerd {
namespace slam {
namespace v2 {

class KinectReaderV2 : public KinectReader {
 public:
  KinectReaderV2();
  KinectReaderV2(const KinectConfigV2& config);
  virtual ~KinectReaderV2();
  virtual bool FindDevice() override;
  virtual void Run() override;
  virtual void StartDevice() override;
  virtual void PauseDevice() override;
  virtual void StopDevice() override;

 private:
  void Loop();
  void ProcessRGBFrame(const libfreenect2::Frame* rgb);
  void ProcessIRFrame(const libfreenect2::Frame* ir);
  void ProcessDepthFrame(const libfreenect2::Frame* depth);
  void Register(const libfreenect2::Frame* rgb,
                const libfreenect2::Frame* depth);

  KinectConfigV2 config_;

  libfreenect2::Freenect2 freenect2_;
  libfreenect2::Logger* logger_;
  libfreenect2::Freenect2Device* device_;
  libfreenect2::PacketPipeline* pipeline_;
  libfreenect2::FrameMap frame_map_;
  libfreenect2::SyncMultiFrameListener* listener_;
  libfreenect2::Registration* registration_;
  libfreenect2::Frame* undistorted_;
  libfreenect2::Frame* registered_;
};

} /* end of v2 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_READER_V2_H */
