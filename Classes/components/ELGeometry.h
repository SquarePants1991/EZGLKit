//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELGEOMETRY_H
#define EZGL_ELGEOMETRY_H

#include "core/ELComponent.h"
#include "types/ELGeometryTypes.h"

typedef struct {
    ELUint vertexVBO;
    ELUint indiceVBO;
    ELSize indiceCount;
    ELUint vertexStride;
    ELUint vertexCount;
    ELBool supportIndiceVBO;
    ELBool supportColorAttrib;
}ELGeometryData;

class ELEffect;
class ELCamera;

class ELGeometry : public ELComponent {
public:
    ELMaterial material;

    bool enableBorder;
    ELVector4 borderColor;
    ELFloat borderWidth;

    bool onlyUseColorAttrib;
public:
    ELGeometry();
    void prepare();

    virtual void update(ELFloat timeInSecs);
    virtual void render();
    virtual std::string kind();

protected:
    ELGeometryData data;
    bool isGeometryDataValid;
    virtual ELGeometryData generateData() = 0;
    void setNeedRegenData();

private:
    ELInt vao;
    void setupVao();
    ELEffect *effect();
};


#endif //EZGL_ELGEOMETRY_H
