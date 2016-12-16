vec3 caculateSurfaceToEyeVec() {
    // 计算表面点到摄像机的向量
    highp vec3 eye;
    eye = normalize(cameraPosition - modelMatrixPosition);
#ifdef Use_BumpMap
    eye = normalize(tbnMatrix * eye);
#endif
    return eye;
}