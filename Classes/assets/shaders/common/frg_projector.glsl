uniform projector_struct projectors[4];
uniform sampler2D projectorMap;
uniform int projectorNum;

vec4 project(projector_struct projector) {
    highp vec4 projectorMVPPosition = projector.viewProjection * modelMatrix * fragPosition;
    projectorMVPPosition = projectorMVPPosition / projectorMVPPosition.w;
    projectorMVPPosition = projectorMVPPosition * 0.5 + 0.5;
    vec4 projectColor;
    if (projectorMVPPosition.x >= 0.0 && projectorMVPPosition.x <= 1.0
    && projectorMVPPosition.y < 1.0 && projectorMVPPosition.y > 0.0) {
        projectColor = tex2D(projectorMap, projectorMVPPosition.st);
    } else {
        projectColor = vec4(0.0,0.0,0.0,0.0);
    }
    return projectColor;
}

bool caculateProjector(out vec4 out_ambient, out vec4 out_diffuse, out vec4 out_specular) {
    out_ambient = vec4(0.0,0.0,0.0,0.0);
    out_diffuse = vec4(0.0,0.0,0.0,0.0);
    out_specular = vec4(0.0,0.0,0.0,0.0);
    for(int i=0;i<projectorNum;i++) {
        out_diffuse += project(projectors[i]);
    }
    return projectorNum > 0;
}