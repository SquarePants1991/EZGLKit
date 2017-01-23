//
//  Shader.fsh
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

precision highp float;

varying highp vec4 fragPosition;

void main()
{
    gl_FragColor = normalize(fragPosition);
}
