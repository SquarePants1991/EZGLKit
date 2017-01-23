//
// Created by wangyang on 16/12/29.
//

#include "lua_ELWorld.h"

void lua_register_world(lua_State *state) {
    lua_newtable(state);

    lua_pushcfunction(state, lua_new_world);
    lua_setfield(state,-2,"new");

    lua_setglobal(state, "World");
}

ELWorld *lua_world_from_table(lua_State *state, int idx) {
    lua_getfield(state, idx, "this");
    ELWorld *world = (ELWorld *)lua_touserdata(state, -1);
    return world;
}

int lua_new_world(lua_State *state) {
    ELWorld *world = new ELWorld();
    lua_create_table_from_world(state, world);
    return 1;
}

void lua_push_world(lua_State *state, ELWorld *world) {
    lua_create_table_from_world(state, world);
}

// Instance Method

int lua_create_table_from_world(lua_State *state, ELWorld *world) {
    lua_newtable(state);
    lua_pushlightuserdata(state, world);
    lua_setfield(state,-2,"this");

    lua_pushcfunction(state, lua_add_node);
    lua_setfield(state,-2,"addNode");
    return 0;
}

// addNode()
int lua_add_node(lua_State *state) {
    ELWorld *world = lua_world_from_table(state, 1);
    ELNode *node = (ELNode *)lua_touserdata(state, 2);
    world->addNode(node);
    return 0;
}

