//
// Created by wangyang on 16/12/29.
//

#ifndef EZGL_LUA_ELWORLD_H
#define EZGL_LUA_ELWORLD_H

#include "core/ELWorld.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static int lua_create_table_from_world(lua_State *state, ELWorld *world);
static ELWorld *lua_world_from_table(lua_State *state, int idx);
static int lua_new_world(lua_State *state);
static void lua_push_world(lua_State *state, ELWorld *world);
static void lua_register_world(lua_State *state);

// Instance Methods, First param will be table
static int lua_add_node(lua_State *state);




#endif //EZGL_LUA_ELWORLD_H
