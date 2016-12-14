//
//  Shader.vsh
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texcoord;

varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec4 fragPosition;

uniform mat4 viewProjection;
uniform mat4 modelMatrix;

void main()
{
    fragTexcoord = texcoord;
    fragNormal = normal;
    fragPosition = position;
    
    gl_Position = viewProjection * modelMatrix * position;
}
