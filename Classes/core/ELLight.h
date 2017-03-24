//
// Created by wangyang on 16/11/25.
//

#ifndef EZGL_ELLIGHT_H
#define EZGL_ELLIGHT_H

#include "../EZGLBase.h"
#include "ELNode.h"
#include "ELCamera.h"

enum _ELLightType {
    ELLightTypePoint = 0,
    ELLightTypeDirection
};
typedef _ELLightType ELLightType;

class ELLight : public ELNode {
public:
    ELLightType type;
    ELVector4 color;
    ELFloat intensity;
    ELVector3 position;
    ELFloat intensityFallOff;
    bool isShadowEnabled;
    ELInt shadowTexture;
public:
    ELLight();
    void enableShadow();
    void disableShadow();
    ELCamera *shadowMapGenCamera();

    void beginGenShadowMap();
    void endGenShadowMap();

    virtual std::string kind();
    virtual void update(ELFloat timeInSecs);

protected:
    ~ELLight();
private:
    ELUint shadowFramebuffer;
    ELCamera * cameraForShadowMap;
    void createShadowFramebuffer();
    void genDepthFramebuffer();
    void genColorFramebuffer();
};


#endif //EZGL_ELLIGHT_H
