//
// Created by wangyang on 16/12/12.
//

#ifndef EZGL_ELTERRAIN_H
#define EZGL_ELTERRAIN_H

#include "ELGeometry.h"

class ELGeometryVertexBuffer;
class ELTerrain : public ELGeometry {
public:
    float *mapData;
    ELVector2 mapDataSize;
    ELFloat resolution;
    ELVector2 size;
    ELFloat maxHeight;
public:
    ELTerrain(ELVector2 size, std::string heightMapPath);
    ELTerrain(ELVector2 size, std::string heightMapPath, ELFloat maxHeight);
    void genMap();
    virtual ELGeometryData generateData();

private:
    ELGeometryVertexBuffer *vertexBuffer;
    int mapWidth;
    int mapHeight;
    unsigned char *imageData;

    ~ELTerrain();
    ELFloat heightWithLocation(ELFloat x,ELFloat y);
};


#endif //EZGL_ELTERRAIN_H
