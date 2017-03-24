//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELGEOMETRY_H
#define EZGL_ELGEOMETRY_H

#include "../core/ELComponent.h"
#include "../types/ELGeometryTypes.h"

typedef struct {
    ELUint vertexVBO;
    ELUint indiceVBO;
    ELSize indiceCount;
    ELUint vertexStride;
    ELUint vertexCount;
    ELBool supportIndiceVBO;
    ELBool supportColorAttrib;
}ELGeometryData;

typedef enum {
    ELGeometryRenderTypeDoubleSide,
    ELGeometryRenderTypeFrontSide,
    ELGeometryRenderTypeBackSide,
}ELGeometryRenderType;

class ELAnimation {
public:
    std::string name;
    ELFloat startTime;
    ELFloat stopTime;
    static ELAnimation None() {
        ELAnimation animation;
        animation.name = "";
        return animation;
    }
    
    static ELAnimation Default() {
        ELAnimation animation;
        animation.name = "Action_Default";
        animation.startTime = 0;
        animation.stopTime = 10000;
        return animation;
    }

    bool operator==(ELAnimation animation) {
        return name == animation.name;
    }

    bool operator!=(ELAnimation animation) {
        return name != animation.name;
    }
};

class ELEffect;
class ELCamera;

class ELGeometry : public ELComponent {
public:
    ELVector3 size;
    ELMaterial material;
    ELMaterial materials[4];
    ELGeometryRenderType renderType;

    bool enableBorder;
    ELVector4 borderColor;
    ELFloat borderWidth;

    bool onlyUseColorAttrib;
    static bool resetBorderBeforeUpdate;

public:
    ELGeometry();
    void prepare();

    virtual void update(ELFloat timeInSecs);
    virtual void render();
    virtual void effectDidActive(ELEffect * effect) {}
    virtual void effectDidInactive(ELEffect * effect) {}
    virtual std::string kind();

protected:
    ELGeometryData data;
    bool isGeometryDataValid;
    virtual ELGeometryData generateData() = 0;
    virtual ELMatrix4 modelMatrix();
    void setNeedRegenData();
private:
    ELInt vao;
    void setupVao();
    ELEffect *effect();
};


#endif //EZGL_ELGEOMETRY_H
