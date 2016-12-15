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