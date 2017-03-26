vec4 terrainColor() {
    // diffuse 0 ~ 0.3
    // ambient 0.3 ~ 0.7
    // specular 0.7 ~ 1
    int matId = int(fragMatID);
    vec4 color1 = tex2D(materials[matId].diffuseMap, fragTexcoord);
    vec4 color2 = tex2D(materials[matId].normalMap, fragTexcoord);
    vec4 color3 = vec4(.0,0.0,1.0,1.0);//tex2D(specularMap, fragTexcoord);
    highp vec4 lightMVPPosition = modelMatrix * fragPosition;
    float bottom = -15.0;
    float top = 35.0;
    float percent1 = (lightMVPPosition.y - bottom) / 70.0 / 0.4;
    percent1 = percent1 > 1.0 ? 1.0 : percent1;
    percent1 = percent1 < 0.0 ? 0.0 : percent1;
    percent1 = 1.0-percent1;

    float percent2 = (lightMVPPosition.y - bottom - 70.0 * 0.2) / 70.0 / 0.8;
    percent2 = percent2 > 1.0 ? 1.0 : percent2;
    percent2 = percent2 < 0.0 ? 0.0 : percent2;

    vec4 finalColor = vec4((color1 * percent1 + color2 * (1.0 - percent1)).rgb,1.0);
    finalColor = vec4((finalColor * percent1 + vec4(1.0,1.0,1.0,1.0) * (1.0 - percent1) * 0.8).rgb, 1.0);
    return finalColor;
}
