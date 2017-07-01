#include "NerdSLAM/lua_config.h"
#include <boost/filesystem.hpp>

namespace nerd {
namespace slam {

LuaConfig::LuaConfig(const std::string& filepath) : lua_state_(luaL_newstate()) {
  luaL_openlibs(lua_state_);
  try {
    boost::filesystem::path p{filepath};
    boost::filesystem::file_status s = status(p);
    if (boost::filesystem::is_regular_file(s)) {
      int rs = luaL_dofile(lua_state_, filepath.c_str());
      if (rs != LUA_OK) {
        throw std::exception();
      }
    } else {
      throw std::exception();
    }
  } catch (boost::filesystem::filesystem_error& e) {
    throw e;
  }
}

LuaConfig::~LuaConfig() {
  if (lua_state_) {
    lua_close(lua_state_);
  }
}

template <>
int LuaConfig::Get(const std::string& var_name) {
  if (PrepareStack(var_name)) {
    return lua_tointeger(lua_state_, -1);
  }
  return 0;
}

template <>
float LuaConfig::Get(const std::string& var_name) {
  if (PrepareStack(var_name)) {
    return static_cast<float>(lua_tonumber(lua_state_, -1));
  }
  return 0;
}

template <>
double LuaConfig::Get(const std::string& var_name) {
  if (PrepareStack(var_name)) {
    return lua_tonumber(lua_state_, -1);
  }
  return 0;
}

template <>
std::string LuaConfig::Get(const std::string& var_name) {
  if (PrepareStack(var_name)) {
    return std::string(lua_tostring(lua_state_, -1));
  }
  return 0;
}

template <>
const void* LuaConfig::Get(const std::string& var_name) {
  if (PrepareStack(var_name)) {
    return lua_topointer(lua_state_, -1);
  }
  return 0;
}

bool LuaConfig::PrepareStack(const std::string& var_name) {
  int str_len = var_name.length();
  int last = 0;
  int level = 0;
  for (int i = 0; i < str_len; ++i) {
    if (var_name.at(i) == '.') {
      std::string var = var_name.substr(last, i);
      last = i + 1;
      if (level == 0) {
        lua_getglobal(lua_state_, var.c_str());
      } else {
        lua_getfield(lua_state_, -1, var.c_str());
      }
      if (lua_isnil(lua_state_, -1)) {
        return false;
      }
      ++level;
    }
  }


  std::string var = var_name.substr(last);
  if (level == 0) {
    lua_getglobal(lua_state_, var.c_str());
  } else {
    lua_getfield(lua_state_, -1, var.c_str());
  }
  if (lua_isnil(lua_state_, -1)) {
    return false;
  }
  return true;
}

} /* end of slam namespace */
} /* end of nerd namespace */
