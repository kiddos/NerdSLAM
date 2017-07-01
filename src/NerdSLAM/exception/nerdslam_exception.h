#ifndef NERDSLAM_EXCEPTION_H
#define NERDSLAM_EXCEPTION_H

#include <exception>
#include <string>

namespace nerd {
namespace slam {
namespace except {

class NerdSLAMException : public std::exception {
 public:
  NerdSLAMException() : msg_("NerdSLAMException") {}
  explicit NerdSLAMException(const char* msg)
    : msg_(std::string("NerdSLAMException: ") + msg) {}
  explicit NerdSLAMException(const std::string& msg)
    : msg_(std::string("NerdSLAMException: ") + msg) {}
  virtual ~NerdSLAMException() {}
  virtual const char* what() const noexcept {
    return msg_.c_str();
  }

 protected:
  std::string msg_;
};

} /* end of except namespace */
} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: NERDSLAM_EXCEPTION_H */
