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

vec4 processCluster(vec4 positionIn) {
    mat4 finalMatrix = mat4(1.0);
    if (clusterCount > 3.0) {
        mat4 cluster3 = clusters[int(clusterID3)];
        finalMatrix = cluster3;
    }
    if (clusterCount > 2.0) {
        finalMatrix *= clusters[int(clusterID2)];
    }
    if (clusterCount > 1.0) {
        finalMatrix *= clusters[int(clusterID1)];
    }
    if (clusterCount > 0.0) {
        finalMatrix *= clusters[int(clusterID0)];
    }
    
    return finalMatrix * positionIn;
}
