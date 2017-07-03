#include "NerdSLAM/kinect/v2/kinect_display_window.h"
#include <boost/program_options.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  using namespace boost::program_options;
  options_description desc("Kinect Display 2D");
  desc.add_options()("help,h", "help messages");
  desc.add_options()("width",
    value<int>()->default_value(640), "Window Width");
  desc.add_options()("height",
    value<int>()->default_value(480), "Window Height");
  desc.add_options()("debug",
    value<bool>()->default_value(false), "Debug Mode");
  desc.add_options()("major",
    value<int>()->default_value(4), "OpenGL Major Version");
  desc.add_options()("minor",
    value<int>()->default_value(5), "OpenGL Minor Version");

  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);

  if (vmap.count("help")) {
    std::cout << desc << "\n";
    return 0;
  }

  nerd::slam::OpenGLWindowConfig config;
  if (vmap.count("width")) {
    int width = vmap["width"].as<int>();
    config.set_width(width);
  }
  if (vmap.count("height")) {
    int height = vmap["height"].as<int>();
    config.set_height(height);
  }
  if (vmap.count("debug")) {
    bool debug = vmap["debug"].as<bool>();
    config.set_debug(debug);
  }
  if (vmap.count("major")) {
    int major = vmap["major"].as<int>();
    if (major < 1 || major > 4) {
      config.set_major_version(1);
    } else {
      config.set_major_version(major);
    }
  }
  if (vmap.count("minor")) {
    int minor = vmap["minor"].as<int>();
    config.set_minor_version(minor);
  }
  config.set_title("Kinect Display 2D");

  nerd::slam::v2::KinectDisplayWindow window(config);
  if (window.Init()) {
    window.MainLoop();
  }
  return 0;
}
