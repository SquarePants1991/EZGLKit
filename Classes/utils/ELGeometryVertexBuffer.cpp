//
// Created by wangyang on 16/11/25.
//

#include <stdlib.h>
#include <string>
#include "ELGeometryVertexBuffer.h"

ELGeometryVertexBuffer::ELGeometryVertexBuffer() {
    bufferLen = 32;
    index = 0;
    vertices = (ELGeometryVertex *)malloc(sizeof(ELGeometryVertex) * bufferLen);
}

void ELGeometryVertexBuffer::append(ELGeometryVertex vertex) {
    *(vertices + index) = vertex;
    index++;
    if (index > bufferLen * 3 / 4.0) {
        bufferLen += 32;
        vertices = (ELGeometryVertex *)realloc(vertices, sizeof(ELGeometryVertex) * bufferLen);
    }
}

void ELGeometryVertexBuffer::append(ELGeometryRect rect) {
    ELVector3 point1 = rect.point1;
    ELVector3 point2 = rect.point2;
    ELVector3 point3 = rect.point3;
    ELVector3 point4 = rect.point4;


    ELVector3 edge1 = ELVector3Subtract((ELVector3)point1, (ELVector3)point2);
    ELVector3 edge2 = ELVector3Subtract((ELVector3)point4, (ELVector3)point2);
    ELVector3 normal = ELVector3CrossProduct(edge1, edge2);
    normal = ELVector3Normalize(normal);

    ELGeometryVertex vertex1_1 = {point1.x, point1.y, point1.z, normal.x, normal.y, normal.z, rect.uv1.x, rect.uv1.y};
    ELGeometryVertex vertex1_2 = {point4.x, point4.y, point4.z, normal.x, normal.y, normal.z, rect.uv4.x, rect.uv4.y};
    ELGeometryVertex vertex1_3 = {point2.x, point2.y, point2.z, normal.x, normal.y, normal.z, rect.uv2.x, rect.uv2.y};

    ELVector3 tangent1_1,bitangent1_1;
    ELVector3 tangent1_2,bitangent1_2;
    ELVector3 tangent1_3,bitangent1_3;
    caculateTangents(&tangent1_1, &bitangent1_1, (ELVector3)point1, normal);
    caculateTangents(&tangent1_2, &bitangent1_2, (ELVector3)point2, normal);
    caculateTangents(&tangent1_3, &bitangent1_3, (ELVector3)point3, normal);
    vertex1_1.tnx = tangent1_1.x;       vertex1_1.tny = tangent1_1.y;       vertex1_1.tnz = tangent1_1.z;
    vertex1_1.btnx = bitangent1_1.x;    vertex1_1.btny = bitangent1_1.y;    vertex1_1.btnz = bitangent1_1.z;
    vertex1_2.tnx = tangent1_2.x;       vertex1_2.tny = tangent1_2.y;       vertex1_2.tnz = tangent1_2.z;
    vertex1_2.btnx = bitangent1_2.x;    vertex1_2.btny = bitangent1_2.y;    vertex1_2.btnz = bitangent1_2.z;
    vertex1_3.tnx = tangent1_3.x;       vertex1_3.tny = tangent1_3.y;       vertex1_3.tnz = tangent1_3.z;
    vertex1_3.btnx = bitangent1_3.x;    vertex1_3.btny = bitangent1_3.y;    vertex1_3.btnz = bitangent1_3.z;


    append(vertex1_1);
    append(vertex1_2);
    append(vertex1_3);



    edge1 = ELVector3Subtract((ELVector3)point4, (ELVector3)point2);
    edge2 = ELVector3Subtract((ELVector3)point3, (ELVector3)point2);
    normal = ELVector3CrossProduct(edge1, edge2);
    normal = ELVector3Normalize(normal);

    ELGeometryVertex vertex2_1 = {point2.x, point2.y, point2.z, normal.x, normal.y, normal.z, rect.uv2.x, rect.uv2.y};
    ELGeometryVertex vertex2_2 = {point4.x, point4.y, point4.z, normal.x, normal.y, normal.z, rect.uv4.x, rect.uv4.y};
    ELGeometryVertex vertex2_3 = {point3.x, point3.y, point3.z, normal.x, normal.y, normal.z, rect.uv3.x, rect.uv3.y};

    ELVector3 tangent2_1,bitangent2_1;
    ELVector3 tangent2_2,bitangent2_2;
    ELVector3 tangent2_3,bitangent2_3;
    caculateTangents(&tangent2_1, &bitangent2_1, (ELVector3)point1, normal);
    caculateTangents(&tangent2_2, &bitangent2_2, (ELVector3)point4, normal);
    caculateTangents(&tangent2_3, &bitangent2_3, (ELVector3)point3, normal);
    vertex2_1.tnx = tangent2_1.x;       vertex2_1.tny = tangent2_1.y;       vertex2_1.tnz = tangent2_1.z;
    vertex2_1.btnx = bitangent2_1.x;    vertex2_1.btny = bitangent2_1.y;    vertex2_1.btnz = bitangent2_1.z;
    vertex2_2.tnx = tangent2_2.x;       vertex2_2.tny = tangent2_2.y;       vertex2_2.tnz = tangent2_2.z;
    vertex2_2.btnx = bitangent2_2.x;    vertex2_2.btny = bitangent2_2.y;    vertex2_2.btnz = bitangent2_2.z;
    vertex2_3.tnx = tangent2_3.x;       vertex2_3.tny = tangent2_3.y;       vertex2_3.tnz = tangent2_3.z;
    vertex2_3.btnx = bitangent2_3.x;    vertex2_3.btny = bitangent2_3.y;    vertex2_3.btnz = bitangent2_3.z;

    append(vertex2_1);
    append(vertex2_2);
    append(vertex2_3);
}

void ELGeometryVertexBuffer::append(ELGeometryTriangle triangle) {

}


ELSize ELGeometryVertexBuffer::rawLength() {
    return index * sizeof(ELGeometryVertex);
}

std::string idFromGeometryVertex(ELGeometryVertex vertex) {
    ELFloat x = floor(vertex.x * 100);
    ELFloat y = floor(vertex.y * 100);
    ELFloat z = floor(vertex.z * 100);
//return [NSString stringWithFormat:@"%.2f-%.2f-%.2f",x,y,z];
    return "asda";
}

void ELGeometryVertexBuffer::caculatePerVertexNormal() {
//    int sum = index;
//    std::map<std::string, std::vector<ELGeometryVertex *>> collectMap;
//    for (int i = 0; i< sum; i++) {
//        ELGeometryVertex vertex = *(vertices + i);
//        NSString *identity = [self idFromGeometryVertex:vertex];
//        NSArray *points = collectMap[identity];
//        if (points == nil) {
//            points = [NSArray new];
//        }
//        points = [points arrayByAddingObject:[NSValue valueWithPointer:(vertices + i)]];
//        collectMap[identity] = points;
//    }
//
//    for (NSString *key in collectMap.keyEnumerator) {
//        ELVector3 normals = ELVector3Make(0, 0, 0);
//        for (NSValue *vertex in collectMap[key]) {
//            ELGeometryVertex *pVertex = (ELGeometryVertex *)[vertex pointerValue];
//            normals = ELVector3Add(normals, ELVector3Make(pVertex->nx, pVertex->ny, pVertex->nz));
//        }
//        normals = ELVector3Normalize(normals);
//        for (NSValue *vertex in collectMap[key]) {
//            ELGeometryVertex *pVertex = (ELGeometryVertex *)[vertex pointerValue];
//            pVertex->nx = normals.x;
//            pVertex->ny = normals.y;
//            pVertex->nz = normals.z;
//        }
//    }
}
void * ELGeometryVertexBuffer::data() {
    return (void *)vertices;
}

void ELGeometryVertexBuffer::clear() {
    index = 0;
}

void ELGeometryVertexBuffer::caculateTangents(ELVector3 *pTangent, ELVector3 *pBitangent, ELVector3 position, ELVector3 normal) {
    ELFloat x = position.x;
    ELFloat y = position.y;
    ELFloat z = position.z;
    ELFloat nx = normal.x;
    ELFloat ny = normal.y;
    ELFloat nz = normal.z;

    ELFloat x1 = x + 1;ELFloat z1 = z + 1;ELFloat y1 = ny == 0 ? y : (nz * (z - z1) + nx * (x - x1))/ny + y;
    ELVector3 resultY = ELVector3Make(x1 - x, y1 - y, z1 - z);

    x1 = x + 1;y1 = y + 1;z1 = nz == 0 ? z : (ny * (y - y1) + nx * (x - x1))/nz + z;
    ELVector3 resultZ = ELVector3Make(x1 - x, y1 - y, z1 - z);

    z1 = z + 1;y1 = y + 1;x1 = nx == 0 ? x : (ny * (y - y1) + nz * (z - z1))/nx + x;
    ELVector3 resultX = ELVector3Make(x1 - x, y1 - y, z1 - z);

    if (resultX.x < 50) {
        *pTangent = resultX;
        *pBitangent = ELVector3CrossProduct(resultX,normal);
    } else if (resultY.y < 50) {
        *pTangent = resultY;
        *pBitangent = ELVector3CrossProduct(resultY,normal);
    } else {
        *pTangent = resultZ;
        *pBitangent = ELVector3CrossProduct(resultZ,normal);
    }
}