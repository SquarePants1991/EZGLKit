//
// Created by wangyang on 16/12/9.
//

#ifndef EZGL_ELWATERPLANE_H
#define EZGL_ELWATERPLANE_H

#include "EZGLBase.h"
#include "ELGeometry.h"
#include "core/ELCamera.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELWaterPlane : public ELGeometry {
public:
    GLuint dudvMap;
    GLuint reflectionMap;
    ELVector2 size;
    ELWaterPlane();
    ELWaterPlane(ELVector2 size);

    ELVector3 plane();
    void beginGenReflectionMap();
    void endGenReflectionMap();
    virtual void render();
    virtual ELGeometryData generateData();
    virtual std::string kind();

private:
    ELGeometryVertexBuffer *vertexBuffer;
    ELUint reflectionFramebuffer;
    ELCamera * cameraForShadowMap;
    void createReflectionFramebuffer();
    void genDepthFramebuffer();
    void genColorFramebuffer();
};



#endif //EZGL_ELWATERPLANE_H
