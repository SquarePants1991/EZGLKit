//
// Created by wangyang on 16/12/12.
//

#include <stdlib.h>
#include "../core/ELTexture.h"
#include "../core/ELEffect.h"
#include "../core/ELGameObject.h"

#include "ELTerrain.h"
#include "../utils/ELGeometryVertexBuffer.h"

ELTerrain::ELTerrain(ELVector2 size, std::string heightMapPath) :
        size(size),
        mapData(NULL),
        resolution(1.0f),
        maxHeight(100),
        imageData(NULL)
{
    this->size = size;
    ELTexture *texture = ELTexture::texture(heightMapPath.c_str(), true);
    imageData = texture->imgData;
    mapWidth = texture->width;
    mapHeight = texture->height;
}

ELTerrain::ELTerrain(ELVector2 size, std::string heightMapPath, ELFloat maxHeight) :
        size(size),
        mapData(NULL),
        resolution(1.0f),
        maxHeight(maxHeight),
        imageData(NULL)
{
    this->size = size;
    this->maxHeight = maxHeight;
    ELTexture *texture = ELTexture::texture(heightMapPath.c_str(), true);
    imageData = texture->imgData;
    mapWidth = texture->width;
    mapHeight = texture->height;
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
        if (imageX > mapWidth - 1) {
            imageX = mapWidth - 1;
        }
        if (imageY > mapHeight - 1) {
            imageY = mapHeight - 1;
        }
        int bytesPerPixel = 4;// R G B A  4 bytes
        int bytesPerRow = mapWidth * bytesPerPixel;
        int loc = imageY * bytesPerRow + imageX * bytesPerPixel;
        unsigned char rComponent = *(imageData + loc);
        return rComponent / 255.0 * maxHeight;
    }
    return 0;
}

void ELTerrain::genMap() {
    if (!vertexBuffer) {
        vertexBuffer = retain_ptr_init(ELGeometryVertexBuffer);
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
    vertexBuffer->caculatePerVertexNormal();
    printf("map data gen => 0x%x, size %f, %f",mapData,size.x,size.y);
}

ELGeometryData ELTerrain::generateData() {
    if (!vertexBuffer) {
        genMap();
    }

    data.vertexVBO = vertexBuffer->getVBO();
    data.vertexCount = vertexBuffer->rawLength() / sizeof(ELGeometryVertex);
    data.vertexStride = sizeof(ELGeometryVertex);
    data.supportIndiceVBO = false;
    data.supportColorAttrib = false;
    return data;
}

void ELTerrain::effectDidActive(ELEffect * effect) {
  glUniform1i(effect->program->uniform("enableTerrainMix"),1);
}

void ELTerrain::render() {
    ELGeometry::render();
    glUniform1i(gameObject()->activeEffect()->program->uniform("enableTerrainMix"),0);
}
