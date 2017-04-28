uniform int enableClipPlane0;
uniform vec4 clipPlane0;
#ifndef ES
float gl_ClipDistance[1];
#endif

void clipPlane(vec4 positionIn) {
    if (enableClipPlane0 == 1)
    {
        #ifndef ES
        gl_ClipDistance[0] = dot(modelMatrix * positionIn, clipPlane0);
        #endif
    }
}
