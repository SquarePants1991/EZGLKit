//
//  Shader.fsh
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

precision highp float;

varying lowp vec2 fragTexcoord;

uniform sampler2D diffuse;

void main()
{
    gl_FragColor = texture2D(diffuse,fragTexcoord);
}
