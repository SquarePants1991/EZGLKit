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
    static bool isInWaterPlanePreparePass;

    GLuint dudvMap;
    GLuint reflectionMap;
    GLuint refractionMap;
    ELVector2 size;
    ELVector4 waterColor;

    ELWaterPlane();
    ELWaterPlane(ELVector2 size);

    ELVector4 plane();
    ELVector4 inversePlane();
    void beginGenReflectionMap();
    void endGenReflectionMap();
    void beginGenRefractionMap();
    void endGenRefractionMap();
    virtual void render();
    virtual void effectDidActive(ELEffect * effect);
    virtual void effectDidInactive(ELEffect * effect);
    virtual ELGeometryData generateData();
    virtual std::string kind();

private:
    ELGeometryVertexBuffer *vertexBuffer;
    ELUint reflectionFramebuffer;
    ELUint refractionFramebuffer;
    void createFramebuffers();
    void genColorFramebuffer(GLuint &outFramebuffer,GLuint &outTexture, ELUint width, ELUint height);
};



#endif //EZGL_ELWATERPLANE_H
