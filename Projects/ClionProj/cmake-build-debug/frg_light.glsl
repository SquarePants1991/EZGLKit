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

void caculateLights(vec3 normal,vec3 eyeVec, out vec4 out_ambient, out vec4 out_diffuse, out vec4 out_specular) {
    highp vec4 sum_ambient = vec4(0.0,0.0,0.0,0.0), sum_diffuse = vec4(0.0,0.0,0.0,0.0), sum_specular = vec4(0.0,0.0,0.0,0.0);
    for (int i=0; i< lightNum; i++) {
        // 计算表面点到光源的向量
        light defaultLight = lights[i];
        highp vec3 vp = normalize(defaultLight.position - modelMatrixPosition);
    #ifdef Use_BumpMap
        vp = normalize(tbnMatrix * vp);
    #endif

        // 计算表面到光源的位置
        float lightDistance = distance(defaultLight.position,modelMatrixPosition);

        // 计算光源
        highp vec4 ambient, diffuse, specular;
    #ifdef Use_SpecularMap
        highp vec4 specularColor = tex2D(specularMap, fragTexcoord);
    #else
        highp vec4 specularColor = material.specular;
    #endif
        pointLight(normal,ambient,diffuse,specular,vp,eyeVec,lightDistance,material.ambient,defaultLight.color,vec4(1.0,1.0,1.0,1.0));

        sum_ambient = ambient;
        sum_diffuse = sum_diffuse + diffuse;
        sum_specular = sum_specular + specular;
    }
    out_ambient = sum_ambient;
    out_diffuse = sum_diffuse;
    out_specular = sum_specular;
}