//
// Created by wangyang on 16/11/25.
//

#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <types/ELGeometryTypes.h>
#include "ELGeometryVertexBuffer.h"

ELGeometryVertexBuffer::ELGeometryVertexBuffer() : supportColorAttrib(false), vbo(-1) {
    bufferLen = 32;
    index = 0;
    vertices = (ELGeometryVertex *)malloc(sizeof(ELGeometryVertex) * bufferLen);
    aabbBox.data[0] = INT32_MAX;
    aabbBox.data[1] = INT32_MIN;
    aabbBox.data[2] = INT32_MAX;
    aabbBox.data[3] = INT32_MIN;
    aabbBox.data[4] = INT32_MAX;
    aabbBox.data[5] = INT32_MIN;
}

void ELGeometryVertexBuffer::enableColorAttrib() {
    supportColorAttrib = true;
    colorVertices = (ELGeometryColorVertex *)malloc(sizeof(ELGeometryColorVertex) * bufferLen);
}

void ELGeometryVertexBuffer::append(ELGeometryVertex vertex) {
    ELVector3 tangent, bitangent;
    caculateTangents(&tangent, &bitangent, ELVector3Make(vertex.x, vertex.y, vertex.z),ELVector3Make(vertex.nx, vertex.ny, vertex.nz));
    vertex.tnx = tangent.x;     vertex.tny = tangent.y;       vertex.tnz = tangent.z;
    vertex.btnx = bitangent.x;    vertex.btny = bitangent.y;      vertex.btnz = bitangent.z;

    *(vertices + index) = vertex;
    index++;
    if (index > bufferLen * 3 / 4.0) {
        bufferLen += 32;
        vertices = (ELGeometryVertex *)realloc(vertices, sizeof(ELGeometryVertex) * bufferLen);
    }
    
    // caculate aabb box
    if (vertex.x < aabbBox.minX) aabbBox.minX = vertex.x;
    if (vertex.x > aabbBox.maxX) aabbBox.maxX = vertex.x;
    if (vertex.y < aabbBox.minY) aabbBox.minY = vertex.y;
    if (vertex.y > aabbBox.maxY) aabbBox.maxY = vertex.y;
    if (vertex.z < aabbBox.minZ) aabbBox.minZ = vertex.z;
    if (vertex.z > aabbBox.maxZ) aabbBox.maxZ = vertex.z;
}

void ELGeometryVertexBuffer::append(ELGeometryColorVertex vertex) {
    ELVector3 tangent, bitangent;
    caculateTangents(&tangent, &bitangent, ELVector3Make(vertex.x, vertex.y, vertex.z),ELVector3Make(vertex.nx, vertex.ny, vertex.nz));
    vertex.tnx = tangent.x;     vertex.tny = tangent.y;       vertex.tnz = tangent.z;
    vertex.btnx = bitangent.x;    vertex.btny = bitangent.y;      vertex.btnz = bitangent.z;

    *(colorVertices + index) = vertex;
    index++;
    if (index > bufferLen * 3 / 4.0) {
        bufferLen += 32;
        colorVertices = (ELGeometryColorVertex *)realloc(colorVertices, sizeof(ELGeometryColorVertex) * bufferLen);
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

void ELGeometryVertexBuffer::append(ELGeometryColorRect colorRect) {
    ELGeometryRect rect = colorRect.geometryRect;
    ELVector3 point1 = rect.point1;
    ELVector3 point2 = rect.point2;
    ELVector3 point3 = rect.point3;
    ELVector3 point4 = rect.point4;


    ELVector3 edge1 = ELVector3Subtract((ELVector3)point1, (ELVector3)point2);
    ELVector3 edge2 = ELVector3Subtract((ELVector3)point4, (ELVector3)point2);
    ELVector3 normal = ELVector3CrossProduct(edge1, edge2);
    normal = ELVector3Normalize(normal);

    ELGeometryColorVertex vertex1_1 = {point1.x, point1.y, point1.z, normal.x, normal.y, normal.z, rect.uv1.x, rect.uv1.y};
    ELGeometryColorVertex vertex1_2 = {point4.x, point4.y, point4.z, normal.x, normal.y, normal.z, rect.uv4.x, rect.uv4.y};
    ELGeometryColorVertex vertex1_3 = {point2.x, point2.y, point2.z, normal.x, normal.y, normal.z, rect.uv2.x, rect.uv2.y};
    vertex1_1.color = colorRect.color1;
    vertex1_2.color = colorRect.color4;
    vertex1_3.color = colorRect.color2;

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

    ELGeometryColorVertex vertex2_1 = {point2.x, point2.y, point2.z, normal.x, normal.y, normal.z, rect.uv2.x, rect.uv2.y};
    ELGeometryColorVertex vertex2_2 = {point4.x, point4.y, point4.z, normal.x, normal.y, normal.z, rect.uv4.x, rect.uv4.y};
    ELGeometryColorVertex vertex2_3 = {point3.x, point3.y, point3.z, normal.x, normal.y, normal.z, rect.uv3.x, rect.uv3.y};

    vertex2_1.color = colorRect.color2;
    vertex2_2.color = colorRect.color4;
    vertex2_3.color = colorRect.color3;

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

void ELGeometryVertexBuffer::append(ELGeometryTriangle triangle, ELInt matID) {
    triangle.matID = matID;
    ELVector3 point1 = triangle.point1;
    ELVector3 point2 = triangle.point2;
    ELVector3 point3 = triangle.point3;


    ELVector3 edge1 = ELVector3Subtract((ELVector3)point1, (ELVector3)point2);
    ELVector3 edge2 = ELVector3Subtract((ELVector3)point3, (ELVector3)point2);
    ELVector3 normal = ELVector3CrossProduct(edge1, edge2);
    normal = ELVector3Normalize(normal);

    ELGeometryVertex vertex1_1 = {point1.x, point1.y, point1.z, normal.x, normal.y, normal.z, triangle.uv1.x, triangle.uv1.y};
    ELGeometryVertex vertex1_2 = {point3.x, point3.y, point3.z, normal.x, normal.y, normal.z, triangle.uv3.x, triangle.uv3.y};
    ELGeometryVertex vertex1_3 = {point2.x, point2.y, point2.z, normal.x, normal.y, normal.z, triangle.uv2.x, triangle.uv2.y};

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

    vertex1_1.matID = matID;
    vertex1_2.matID = matID;
    vertex1_3.matID = matID;

    append(vertex1_1);
    append(vertex1_2);
    append(vertex1_3);
}


ELSize ELGeometryVertexBuffer::rawLength() {
    if (supportColorAttrib) {
        return index * sizeof(ELGeometryColorVertex);
    }
    return index * sizeof(ELGeometryVertex);
}

std::string idFromGeometryVertex(ELGeometryVertex vertex) {
    ELFloat x = floor(vertex.x * 100);
    ELFloat y = floor(vertex.y * 100);
    ELFloat z = floor(vertex.z * 100);
    char buffer[512];
    snprintf(buffer, 512, "%.2f-%.2f-%.2f", x, y, z);
    return std::string(buffer);
}

void ELGeometryVertexBuffer::caculatePerVertexNormal() {
    int sum = index;
    std::map<std::string, std::vector<ELGeometryVertex *> > collectMap;
    for (int i = 0; i< sum; i++) {
        ELGeometryVertex vertex = *(vertices + i);
        std::string identity = idFromGeometryVertex(vertex);
        std::vector<ELGeometryVertex *> points = collectMap[identity];
        points.push_back((ELGeometryVertex *)(vertices + i));
        collectMap[identity] = points;
    }
    for(std::map<std::string, std::vector<ELGeometryVertex *> >::iterator iterator = collectMap.begin(); iterator != collectMap.end(); iterator++) {
        std::string key = iterator->first;
        std::vector<ELGeometryVertex *> vertices = iterator->second;

        ELVector3 normals = ELVector3Make(0, 0, 0);
        for (std::vector<ELGeometryVertex *>::iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); vertexIter++) {
            ELGeometryVertex *pVertex = *vertexIter;
            normals = ELVector3Add(normals, ELVector3Make(pVertex->nx, pVertex->ny, pVertex->nz));
        }
        normals = ELVector3Normalize(normals);
        for (std::vector<ELGeometryVertex *>::iterator vertexIter = vertices.begin(); vertexIter != vertices.end(); vertexIter++) {
            ELGeometryVertex *pVertex = *vertexIter;
            pVertex->nx = normals.x;
            pVertex->ny = normals.y;
            pVertex->nz = normals.z;
        }
    }
}
void * ELGeometryVertexBuffer::data() {
    if (supportColorAttrib) {
        return (void *)colorVertices;
    }
    return (void *)vertices;
}

void ELGeometryVertexBuffer::clear() {
    index = 0;
}

ELVector3 ELGeometryVertexBuffer::size() {
    ELVector3 size;
    size.x = aabbBox.maxX - aabbBox.minX;
    size.y = aabbBox.maxY - aabbBox.minY;
    size.z = aabbBox.maxZ - aabbBox.minZ;
    return size;
}

GLuint ELGeometryVertexBuffer::getVBO() {
    GLuint vboVal = vbo;
    if (vbo < 0) {
        glGenBuffers(1, &vboVal);
        vbo = vboVal;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vboVal);
    glBufferData(GL_ARRAY_BUFFER, rawLength(), data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
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
