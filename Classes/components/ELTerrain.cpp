//
// Created by wangyang on 16/12/12.
//

#include <stdlib.h>
#include <SOIL.h>

#include "ELTerrain.h"
#include "utils/ELGeometryVertexBuffer.h"

ELTerrain::ELTerrain(ELVector2 size, std::string heightMapPath) :
        size(size),
        vertexBuffer(NULL),
        mapData(NULL),
        resolution(1.0f),
        maxHeight(100),
        imageData(NULL)
{
    int channels;
    this->size = size;
    imageData = SOIL_load_image(heightMapPath.c_str(),&mapWidth,&mapHeight,&channels,SOIL_LOAD_RGBA);
}

ELTerrain::ELTerrain(ELVector2 size, std::string heightMapPath, ELFloat maxHeight) :
        size(size),
        vertexBuffer(NULL),
        mapData(NULL),
        resolution(1.0f),
        maxHeight(maxHeight),
        imageData(NULL)
{
    this->size = size;
    this->maxHeight = maxHeight;
    int channels;
    imageData = SOIL_load_image(heightMapPath.c_str(),&mapWidth,&mapHeight,&channels,SOIL_LOAD_RGBA);
}

ELTerrain::~ELTerrain() {
    if (mapData != NULL) {
        free(mapData);
    }
}

ELFloat ELTerrain::heightWithLocation(ELFloat xLoc,ELFloat zLoc) {
    if (imageData != NULL) {
        int imageX = (int)(mapWidth * (xLoc + size.x / 2) / size.x);
        int imageY = (int)(mapHeight * (zLoc + size.y / 2) / size.y);
        int bytesPerPixel = 4;// R G B A  4 bytes
        int bytesPerRow = mapWidth * bytesPerPixel;
        int loc = imageY * bytesPerRow + imageX * bytesPerPixel;
        unsigned char rComponent = *(imageData + loc);
        return rComponent / 255.0 * maxHeight;
    }
    return 0;
}

void ELTerrain::genMap() {
    if (vertexBuffer == NULL) {
        vertexBuffer = new ELGeometryVertexBuffer();
    } else {
        vertexBuffer->clear();
    }

    int xCount = (int)(size.x / resolution);
    int yCount = (int)(size.y / resolution);

    if (mapData != NULL) {
        free(mapData);
        mapData = NULL;
    }
    mapData = (float *)malloc(sizeof(float) * xCount * yCount);

    for (int y = 0; y < yCount; y++) {
        for (int x = 0; x < xCount; x++) {
            float xLoc = x * resolution - size.x / 2;
            float zLoc = y * resolution - size.y / 2;
            float height = heightWithLocation(xLoc,zLoc);
            mapData[y * xCount + x] = height;

            ELVector3 point1 = ELVector3Make(xLoc, heightWithLocation(xLoc,zLoc) - maxHeight / 2, zLoc);
            ELVector3 point2 = ELVector3Make(xLoc, heightWithLocation(xLoc,zLoc + resolution) - maxHeight / 2, zLoc + resolution);
            ELVector3 point3 = ELVector3Make(xLoc + resolution, heightWithLocation(xLoc + resolution,zLoc + resolution) - maxHeight / 2, zLoc + resolution);
            ELVector3 point4 = ELVector3Make(xLoc + resolution, heightWithLocation(xLoc + resolution,zLoc) - maxHeight / 2, zLoc);

            float scale = 0.1;
            ELGeometryRect rect = {point4,point3,point2,point1,
                                   {scale * y,scale * x},
                                   {scale * y,scale * (x + 1)},
                                   {scale * (y + 1),scale * (x + 1)},
                                   {scale * (y + 1),scale * x}
            };
            vertexBuffer->append(rect);
        }
    }
    mapDataSize = ELVector2Make(xCount,yCount);
//    vertexBuffer->caculatePerVertexNormal();
    printf("map data gen => 0x%x, size %f, %f",mapData,size.x,size.y);
}

ELGeometryData ELTerrain::generateData() {
    if (vertexBuffer == NULL) {
        genMap();
    }

    GLfloat *vertex = (GLfloat *)vertexBuffer->data();

    ELGeometryData data;
    glGenBuffers(1, &data.vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer->rawLength(), vertex, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    data.vertexCount = vertexBuffer->rawLength() / sizeof(ELGeometryData);
    data.vertexStride = sizeof(ELGeometryVertex);
    data.supportIndiceVBO = false;
    return data;

}