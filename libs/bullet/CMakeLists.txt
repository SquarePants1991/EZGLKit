file(GLOB SRC
        "BulletCollision/**/*.h"
        "BulletCollision/**/*.cpp"
        "BulletDynamics/**/*.h"
        "BulletDynamics/**/*.cpp"
        "BulletSoftBody/*.h"
        "BulletSoftBody/*.cpp"
        "LinearMath/*.h"
        "LinearMath/*.cpp"
)

add_library(bullet btBulletCollisionCommon.h btBulletDynamicsCommon.h ${SRC})
