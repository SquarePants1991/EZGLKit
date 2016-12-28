//
// Created by wang yang on 2016/12/3.
//

#ifndef EZGL_ELBOARD_H
#define EZGL_ELBOARD_H

#include "ELGeometry.h"
#include "utils/ELGeometryVertexBuffer.h"

class ELBoard : public ELGeometry {
public:
    ELBoard(ELVector2 size);
    ~ELBoard();

    virtual ELGeometryData generateData();
    virtual void render();
    virtual void update(ELFloat timeInSecs);

private:
    ELVector2 size;
    ELGeometryVertexBuffer *vertexBuffer;
};


#endif //EZGL_ELBOARD_H
