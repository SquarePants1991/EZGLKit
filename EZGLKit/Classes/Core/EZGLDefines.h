//
//  GLDefines.h
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

#ifndef GLDefines_h
#define GLDefines_h

enum {
    UNIFORM_RENDERASSHADOW,
    UNIFORM_VIEWPROJECTION,
    UNIFORM_MODEL_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_AMBIENT,
    UNIFORM_DIFFUSE,
    UNIFORM_SPECULAR,
    UNIFORM_LIGHT_VIEWPROJECTION,
    UNIFORM_LIGHT_COLOR,
    UNIFORM_LIGHT_POSITION,
    UNIFORM_LIGHT_BRIGHTNESS,
    UNIFORM_DIFFUSE_MAP,
    UNIFORM_SHADOW_MAP,
    NUM_UNIFORMS
};

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#endif /* GLDefines_h */
