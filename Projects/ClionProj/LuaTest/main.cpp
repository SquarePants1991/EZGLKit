//
// Created by wangyang on 16/12/13.
//

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <map>
#include <functional>
#include <string>


class PointCls {
public:
    float xx;
    float yy;
    float zz;
    std::map<std::string, int> data;
    PointCls(float xx,float yy) {
        this->xx = xx;
        this->yy = yy;
        zz = 10;
        data["adsad"] = 5;
    }

    std::map<std::string, int> getData() {
        return data;
    }

    static int attr(lua_State *state) {
        PointCls *pcls = (PointCls *)lua_touserdata(state, lua_upvalueindex(1));
        const char *attrName = lua_tostring(state, 1);
        int paramNum = lua_gettop(state);
        if (paramNum == 1) {

#define GetAttr(attr, attrVal) \
        if (strcmp(attrName, attr) == 0) lua_pushnumber(state, pcls->attrVal);

            GetAttr("xx", xx);
            GetAttr("yy", yy);
            GetAttr("zz", zz);
        } else if (paramNum == 2) {
            float num = lua_tonumber(state, 2);
#define SetAttr(attr, attrVal) \
                if (strcmp(attrName, attr) == 0)  pcls->attrVal = num;
            SetAttr("xx",xx);
        }
        return 1;
    }

    static int desc(lua_State *state) {
        const char * arg = lua_tostring(state, 2);
        int c = lua_gettop(state);

        lua_getfield(state, 1, "this");
        PointCls *ptCls = (PointCls *)lua_touserdata(state, -1);
        ptCls->print();

        lua_pop(state,1);
        c = lua_gettop(state);

        lua_getfield(state, 1, "res");
        const char * str = lua_tostring(state, -1);
        printf("%s", str);
    }

    static int create(lua_State *state) {
        int c = lua_gettop(state);
        lua_newtable(state);
        PointCls *ptCls = new PointCls(3,6);

        lua_pushlightuserdata(state, ptCls);
        lua_setfield(state,-2,"this");

        lua_pushcclosure(state, attr, 0);
        lua_setfield(state,-2,"attr");

        lua_pushcclosure(state, desc, 0);
        lua_setfield(state,-2,"desc");

        lua_pushliteral(state, "hello U");
        lua_setfield(state,-2,"res");

        return 1;
    }

    static void registerLua(lua_State *state) {
        int c = lua_gettop(state);
        lua_newtable(state);
        lua_pushcfunction(state, create);
        lua_setfield(state,-2,"create");

        lua_setglobal(state, "PointCls");
    }

    void print() {
        printf("Hellow world");
    }
};

int main(int argc,char **argv) {
    lua_State *lState = luaL_newstate();
    if (lState == NULL) printf("Create Lua State failed!");
    luaL_openlibs(lState);

    luaL_dofile(lState, "/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/LuaTest/test.lua");
    int c = lua_gettop(lState);
    PointCls::registerLua(lState);

    lua_getglobal(lState, "main");
    lua_pushnumber(lState, 33.332);
    lua_pcall(lState,1,0,0);
    c = lua_gettop(lState);
    return 1;
}