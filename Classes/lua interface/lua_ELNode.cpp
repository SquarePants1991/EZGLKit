//
// Created by wangyang on 16/12/29.
//

#include "lua_ELNode.h"

static void lua_register_node(lua_State *state) {
    lua_newtable(state);

    lua_pushcfunction(state, lua_new_node);
    lua_setfield(state,-2,"new");

    lua_setglobal(state, "Node");
}

static ELNode *lua_node_from_table(lua_State *state, int idx) {
    lua_getfield(state, idx, "this");
    ELNode *node = (ELNode *)lua_touserdata(state, -1);
    return node;
}

static int lua_new_node(lua_State *state) {
    ELNode *node = new ELNode();
    lua_create_table_from_node(state, node);
    return 1;
}

static void lua_push_node(lua_State *state, ELNode *node) {
    lua_create_table_from_node(state, node);
}

// Instance Method

static int lua_create_table_from_node(lua_State *state, ELNode *node) {
    lua_newtable(state);
    lua_pushlightuserdata(state, node);
    lua_setfield(state,-2,"this");

    return 1;
}