/* Pushover
 *
 * Pushover is the legal property of its developers, whose
 * names are listed in the AUTHORS file, which is included
 * within the source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335 USA
 */

#include "luaclass.h"

extern "C" {
#include <lauxlib.h>
}

#include <string.h>

luaClass_c::luaClass_c(void) {

  L = luaL_newstate();

}
luaClass_c::~luaClass_c(void) {

  lua_close(L);

}

unsigned int luaClass_c::getArraySize(const std::string & name) {
  lua_getglobal(L, name.c_str());
#if LUA_VERSION_NUM > 501
  unsigned int res = lua_rawlen(L, -1);
#else
  unsigned int res = lua_objlen(L, -1);
#endif

  lua_pop(L, 1);
  return res;
}

unsigned int luaClass_c::getArraySize(const std::string & name, unsigned int idx) {
  lua_getglobal(L, name.c_str());
  lua_pushnumber(L, idx);
  lua_gettable(L, -2);
#if LUA_VERSION_NUM > 501
  unsigned int res = lua_rawlen(L, -1);
#else
  unsigned int res = lua_objlen(L, -1);
#endif

  lua_pop(L, 2);
  return res;
}

lua_Number luaClass_c::getNumberArray(const std::string & name, unsigned int idx) {
  lua_getglobal(L, name.c_str());
  lua_pushnumber(L, idx);
  lua_gettable(L, -2);

  lua_Number result = lua_tonumber(L, -1);
  lua_pop(L, 2);

  return result;
}

lua_Number luaClass_c::get2dNumberArray(const std::string & name, unsigned int idx1, unsigned int idx2) {
  lua_getglobal(L, name.c_str());
  lua_pushnumber(L, idx1);
  lua_gettable(L, -2);

  lua_pushnumber(L, idx2);
  lua_gettable(L, -2);

  lua_Number result = lua_tonumber(L, -1);
  lua_pop(L, 3);

  return result;
}

/* functions to evaluate lua code */
int luaClass_c::doFile(const std::string & fname) {
  return luaL_dofile(L, fname.c_str());
}
int luaClass_c::doString(const std::string & code) {
  return luaL_dostring(L, code.c_str());
}

