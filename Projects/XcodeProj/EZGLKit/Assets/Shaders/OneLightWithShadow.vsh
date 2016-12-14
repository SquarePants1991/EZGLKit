//
//  Shader.vsh
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

attribute vec4 position;
attribute vec3 normal;
attribute vec2 uv;

varying highp vec4 frag_position;
varying highp vec3 frag_normal;
varying lowp vec2 frag_uv;

uniform highp mat4 lightViewProjection;
uniform highp mat4 viewProjection;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

uniform lowp int renderAsShadow;

void main()
{
    frag_uv = uv;
    frag_position = position;
    frag_normal = normal;
    
    gl_Position = viewProjection * modelMatrix * position;
}
