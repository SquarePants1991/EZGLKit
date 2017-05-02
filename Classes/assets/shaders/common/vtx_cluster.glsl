#define MaxClusterCount 100
uniform mat4 clusters[MaxClusterCount];

VertIN float clusterCount;
VertIN float clusterID0;
VertIN float clusterID1;
VertIN float clusterID2;
VertIN float clusterID3;
VertIN float clusterWeight0;
VertIN float clusterWeight1;
VertIN float clusterWeight2;
VertIN float clusterWeight3;

void processCluster(vec4 positionIn, vec3 normalIn, out vec4 positionOut, out vec3 normalOut) {
    if (clusterCount == 0.0) {
        positionOut = positionIn;
        normalOut = normalIn;
        return;
    }
    vec3 positionNormalEnd = positionIn.xyz + normalIn;
    mat4 finalMatrix = mat4(0.0);
    float totalWeight = 0.0;
    if (clusterCount > 0.0) {
        finalMatrix += clusters[int(clusterID0)] * clusterWeight0;
        totalWeight += clusterWeight0;
    }
    if (clusterCount > 3.0) {
        finalMatrix += clusters[int(clusterID3)] * clusterWeight3;
        totalWeight += clusterWeight3;
    }
    if (clusterCount > 2.0) {
        finalMatrix += clusters[int(clusterID2)] * clusterWeight2;
        totalWeight += clusterWeight2;
    }
    if (clusterCount > 1.0) {
        finalMatrix += clusters[int(clusterID1)] * clusterWeight1;
        totalWeight += clusterWeight1;
    }
    positionOut = finalMatrix * positionIn / totalWeight;

    vec4 positionNormalEndOut = finalMatrix * vec4(positionNormalEnd, 1.0) / totalWeight;
    normalOut = normalize(positionNormalEndOut.xyz - positionOut.xyz);
}