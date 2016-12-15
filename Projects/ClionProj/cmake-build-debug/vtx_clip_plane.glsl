uniform int enableClipPlane0;
uniform vec4 clipPlane0;
float gl_ClipDistance[1];

void clipPlane() {
    if (enableClipPlane0 == 1)
    {
        gl_ClipDistance[0] = dot(modelMatrix * position, clipPlane0);
    }
}