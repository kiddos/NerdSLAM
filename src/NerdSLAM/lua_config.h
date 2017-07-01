#ifndef LUA_CONFIG_H
#define LUA_CONFIG_H

#include <lua5.2/lauxlib.h>
#include <lua5.2/lua.h>
#include <lua5.2/lualib.h>
#include <string>

namespace nerd {
namespace slam {

class LuaConfig {
 public:
  explicit LuaConfig(const std::string& filepath);
  ~LuaConfig();

  template <typename T>
  T Get(const std::string& var_name);

 private:
  bool PrepareStack(const std::string& var_name);
  lua_State* lua_state_;
};

} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: LUA_CONFIG_H */
