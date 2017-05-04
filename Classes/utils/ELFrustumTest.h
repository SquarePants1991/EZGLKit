//
//  ELFrustumTest.hpp
//  EZGLSDK
//
//  Created by wangyang on 2017/5/4.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#ifndef ELFrustumTest_hpp
#define ELFrustumTest_hpp

#include "../types/EZGLTypes.h"
#include "ELGeometryVertexBuffer.h"

extern bool isInViewFrustum(ELMatrix4 frustumMatrix, ELVector3 size, ELMatrix4 modelMatrix);

#endif /* ELFrustumTest_hpp */
