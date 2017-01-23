//
//  Shader.vsh
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

attribute vec4 position;
attribute vec3 normal;      //法线向量，本shader尚未使用
attribute vec2 texcoord;    //纹理坐标 uv，本shader尚未使用

varying highp vec4 fragPosition;

uniform mat4 viewProjection;
uniform mat4 modelMatrix;

void main()
{
    fragPosition = position;
    gl_Position = viewProjection * modelMatrix * position;
}
