void pointLight(
                in vec3 normal,
                out vec4 ambient,
                out vec4 diffuse,
                out vec4 specular,
                in vec3 vp,
                in vec3 eye,
                in float lightDistance,
                in vec4 lightAmbient,
                in vec4 lightDiffuse,
                in vec4 lightSpecular
                ) {
    ambient = lightAmbient;
    vec3 halfVector = normalize(vp + eye);
    float shininess = 60.0;
    float nDotViewPosition = clamp(dot(normal,vp),0.0,1.0);
    diffuse = lightDiffuse * nDotViewPosition;
    float nDotViewHalfVector = clamp(dot(normal,halfVector),0.0,1.0);
    float powerFactor = max(0.0, pow(nDotViewHalfVector,shininess));
    specular = lightSpecular * powerFactor / 10.0;
}