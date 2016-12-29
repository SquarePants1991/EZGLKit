//
// Created by wangyang on 16/12/29.
//

#ifndef EZGL_LUA_ELNODE_H
#define EZGL_LUA_ELNODE_H

#include <core/ELNode.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

extern int lua_create_table_from_node(lua_State *state, ELNode *node);

extern ELNode *lua_node_from_table(lua_State *state, int idx);

extern int lua_new_node(lua_State *state);

extern void lua_push_node(lua_State *state, ELNode *world);

extern void lua_register_node(lua_State *state);


#endif //EZGL_LUA_ELNODE_H
