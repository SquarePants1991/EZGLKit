//
// Created by wangyang on 16/12/13.
//

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <map>
#include <functional>
#include <string>


template <typename Cls,typename T1,typename T2>
class LuaFunc {
public:
    typedef void (func1)(T1 t1);
    typedef void (func2)(T1 t1, T2 t2);
    func1 *fc1;
    func2 *fc2;
    LuaFunc(func1 *fc) {
        fc1 = fc;
    }

    LuaFunc(func2 *fc) {
        fc2 = fc;
    }

    void invoke(Cls *that, T1 t1) {
        (that->*fc1)(t1);
    }
};

class PointCls {
public:
    float xx;
    float yy;
    std::map<std::string,lua_CFunction > mapFunc;
    PointCls(float xx,float yy) {
        this->xx = xx;
        this->yy = yy;
    }


    static int lua_invoke(lua_State *state) {
        PointCls *pcls = (PointCls *)lua_touserdata(state, lua_upvalueindex(1));
        std::string name = lua_tostring(state, lua_upvalueindex(2));
        pcls->lua_func(name)(state);
    }

    void lua_ref (lua_State *state) {
        lua_newtable(state);
        lua_pushlightuserdata(state, (void *)this);
        lua_setfield(state,1,"this");

        lua_pushlightuserdata(state,this);
        lua_pushliteral(state, "print");
        lua_pushcclosure(state, PointCls::lua_print, 2);
        lua_setfield(state,1,"print");
    }

    lua_CFunction lua_func(std::string name) {
        return *mapFunc[name];
    }

    void print(const char *msg) {
        printf("%s",msg);
    }

    void getValue(lua_State *state, int *outVal, int idx) {
        *outVal = lua_tointeger(state,idx);
    }

    template <typename Cls>
    static void invoke(lua_State *state) {
        Cls *pcls = (Cls *)lua_touserdata(state, lua_upvalueindex(1));
        std::string name = lua_tostring(state, lua_upvalueindex(2));
        pcls->print(name.c_str());
    }

//    template <typename Cls,typename T1>
//    static void invoke(lua_State *state, T1 t1, void ()(T1) * method1) {
//        Cls *pcls = (Cls *)lua_touserdata(state, lua_upvalueindex(1));
//        std::string name = lua_tostring(state, lua_upvalueindex(2));
//        pcls->getValue(state, t1, 1);
//        pcls->print();
//    }

    static int lua_print(lua_State *state) {
        auto a =
        PointCls::invoke<PointCls>(state,&PointCls::print);
    }
};

struct point {
    float x;
    float y;
};

int trace(lua_State *state) {
    size_t stringLen;
    const char * str = lua_tolstring(state,1,&stringLen);
    printf("trace : %s \n",str);
    return 1;
}

int say(lua_State *state) {
    size_t stringLen;
    const char * str = lua_tolstring(state,1,&stringLen);
    printf("say : %s \n",str);
    lua_pushliteral(state,"say complete!");
    return 1;
}



int create(lua_State *state) {
    PointCls *pcls = new PointCls(2.3,2.1);
    pcls->lua_ref(state);
    return 1;
}

int main(int argc,char **argv) {
    lua_State *lState = luaL_newstate();
    if (lState == NULL) printf("Create Lua State failed!");
    luaL_openlibs(lState);


    lua_pushcclosure(lState,trace,0);
    lua_setglobal(lState,"trace");

    lua_pushcclosure(lState,say,0);
    lua_setglobal(lState,"say");

    lua_register(lState,"create",create);

    luaL_dofile(lState, "/Users/ocean/Documents/Codes/On Git/EZGLKit/Projects/ClionProj/LuaTest/test.lua");



    lua_getglobal(lState, "main");
    lua_pushnumber(lState, 33.332);
    lua_pcall(lState,1,0,0);

//    PointCls::PrintFunction func = &PointCls::print;
//    PointCls *pc = new PointCls(2,3);
//    (pc->*func)();

    return 1;
}