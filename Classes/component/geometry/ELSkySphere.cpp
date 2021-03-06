//
// Created by wangyang on 16/12/22.
//

#include "ELSkySphere.h"
#include "../../core/ELEffect.h"

ELSkySphere::ELSkySphere(ELFloat radius) :
        radius(radius),
        ring(20),
        segments(20)
{
}

void ELSkySphere::genHalfSphere(ELInt step, ELGeometryVertexBuffer *vertexBuffer) {
    // 半圆
    for (int i = 0;fabs(i) < ring;i+=step) {
        ELFloat ringHeight = radius * i / ring;
        ELFloat ringRadius = sqrt(pow(radius, 2) - pow(ringHeight, 2));
        ELFloat ringHeightNext = radius * (i + step) / ring;
        ELFloat ringRadiusNext = sqrt(pow(radius, 2) - pow(ringHeightNext, 2));

        for (int j=0;j< segments ;j++) {
            int segmentIndex = step > 0 ? j : (segments - j);
            if (segmentIndex == segments) {
                segmentIndex = 0;
            }
            ELFloat radian = segmentIndex / segments * M_PI * 2;
            ELFloat radianNext = (segmentIndex + step) / segments * M_PI * 2;

            ELFloat x_r1_x0 = cos(radian) * ringRadius;
            ELFloat x_r1_y0 = ringHeight;
            ELFloat x_r1_z0 = sin(radian) * ringRadius;
            ELFloat x_r1_x1 = cos(radianNext) * ringRadius;
            ELFloat x_r1_y1 = ringHeight;
            ELFloat x_r1_z1 = sin(radianNext) * ringRadius;

            ELFloat x_r2_x0 = cos(radian) * ringRadiusNext;
            ELFloat x_r2_y0 = ringHeightNext;
            ELFloat x_r2_z0 = sin(radian) * ringRadiusNext;
            ELFloat x_r2_x1 = cos(radianNext) * ringRadiusNext;
            ELFloat x_r2_y1 = ringHeightNext;
            ELFloat x_r2_z1 = sin(radianNext) * ringRadiusNext;

            bool isTriangle = x_r2_x0 == x_r2_x1 && x_r2_z0 == x_r2_z1;

            if (isTriangle) {
                ELGeometryTriangle triangle = {
                        {x_r2_x0, x_r2_y0, x_r2_z0},
                        {x_r1_x0, x_r1_y0, x_r1_z0},
                        {x_r1_x1, x_r1_y1, x_r1_z1},
                        {0.5, 1.0 - (x_r2_y0 + radius) / (radius * 2) * 2.0},
                        {radian / (M_PI * 2.0), 1.0 - (x_r1_y0 + radius) / (radius * 2) * 2.0},
                        {radianNext / (M_PI * 2.0), 1.0 - (x_r1_y1 + radius) / (radius * 2) * 2.0}
                };
                vertexBuffer->append(triangle);
            } else {
                ELGeometryRect rect = {
                        {x_r2_x0, x_r2_y0, x_r2_z0},
                        {x_r1_x0, x_r1_y0, x_r1_z0},
                        {x_r1_x1, x_r1_y1, x_r1_z1},
                        {x_r2_x1, x_r2_y1, x_r2_z1},
                        {radian / M_PI / 2, 1.0 - (x_r2_y0 + radius) / (radius * 2) * 2.0},
                        {radian / M_PI / 2, 1.0 - (x_r1_y0 + radius) / (radius * 2) * 2.0},
                        {radianNext / M_PI / 2, 1.0 - (x_r1_y1 + radius) / (radius * 2) * 2.0},
                        {radianNext / M_PI / 2, 1.0 - (x_r2_y1 + radius) / (radius * 2) * 2.0}
                };
                vertexBuffer->append(rect);
            }
        }
    }
}

void ELSkySphere::fillVertexBuffer(ELGeometryVertexBuffer *vertexBuffer) {
    genHalfSphere(1, vertexBuffer);
}

void ELSkySphere::effectDidActive(ELEffect * effect) {
    glDisable(GL_CULL_FACE);
    glUniform1i(effect->program->uniform("isSky"),1);
}

void ELSkySphere::effectDidInactive(ELEffect * effect) {
    glUniform1i(effect->program->uniform("isSky"),0);
    glEnable(GL_CULL_FACE);
}