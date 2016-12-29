//
// Created by wangyang on 16/12/2.
//

#include <component/geometry/ELSphereGeometry.h>
#include "FGScene.h"

#include "lua interface/lua_Interfaces.h"

void FGScene::update(ELFloat timeInSecs) {

}

void FGScene::mouseLeftButtonClicked() {
}

void FGScene::mouseRightButtonClicked() {

}

static int trace(lua_State *state) {
    printf("%s\n", lua_tostring(state, 1));
}

FGScene::FGScene(ELWorld *world) : world(world){
    lua_State *lState = luaL_newstate();
    if (lState == NULL) printf("Create Lua State failed!");
    luaL_openlibs(lState);
    lua_registerEZGL(lState);

    lua_register(lState, "trace", trace);

    luaL_dofile(lState, ELAssets::shared()->findFile("scene1.lua").c_str());

    lua_push_world(lState, world);
    lua_setglobal(lState, "world");

    lua_getglobal(lState, "init");
    lua_pcall(lState,0,0,0);
}
