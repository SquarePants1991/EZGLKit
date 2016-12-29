//
// Created by wangyang on 16/12/29.
//

#ifndef EZGL_LUA_ELNODE_H
#define EZGL_LUA_ELNODE_H

#include <core/ELNode.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static int lua_create_table_from_node(lua_State *state, ELNode *node);

static ELNode *lua_node_from_table(lua_State *state, int idx);

static int lua_new_node(lua_State *state);

static void lua_push_node(lua_State *state, ELNode *world);

static void lua_register_node(lua_State *state);


#endif //EZGL_LUA_ELNODE_H
