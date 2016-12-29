//
// Created by wangyang on 16/12/29.
//

#include "lua_ELWorld.h"

static void lua_register_world(lua_State *state) {
    lua_newtable(state);

    lua_pushcfunction(state, lua_new_world);
    lua_setfield(state,-2,"new");

    lua_setglobal(state, "World");
}

static ELWorld *lua_world_from_table(lua_State *state, int idx) {
    lua_getfield(state, idx, "this");
    ELWorld *world = (ELWorld *)lua_touserdata(state, -1);
    return world;
}

static int lua_new_world(lua_State *state) {
    ELWorld *world = new ELWorld();
    lua_create_table_from_world(state, world);
    return 1;
}

static void lua_push_world(lua_State *state, ELWorld *world) {
    lua_create_table_from_world(state, world);
}

// Instance Method

static int lua_create_table_from_world(lua_State *state, ELWorld *world) {
    lua_newtable(state);
    lua_pushlightuserdata(state, world);
    lua_setfield(state,-2,"this");

    lua_pushcfunction(state, lua_add_node);
    lua_setfield(state,-2,"addNode");
}

// addNode()
static int lua_add_node(lua_State *state) {
    ELWorld *world = lua_world_from_table(state, 1);
    ELNode *node = (ELNode *)lua_touserdata(state, 2);
    world->addNode(node);
}

