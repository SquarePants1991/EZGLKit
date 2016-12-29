//
// Created by wangyang on 16/12/29.
//

#ifndef EZGL_LUA_INTERFACES_H
#define EZGL_LUA_INTERFACES_H

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lua_ELWorld.h"
//#include "lua_ELNode.h"

static void lua_registerEZGL(lua_State *state) {
    lua_register_world(state);
//    lua_register_node(state);
}

#endif //EZGL_LUA_INTERFACES_H
