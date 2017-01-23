//
//  GameViewController.m
//  EZGL
//
//  Created by wangyang on 2017/1/19.
//  Copyright © 2017年 wangyang. All rights reserved.
//

#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#import "EZGL.h"

#define  Bit(n) (0x00000001 << n)

enum CollisionTypes {
    CT_Floor = Bit(0),
    CT_Prop = Bit(1),
    CT_Prop2 = Bit(2),
    CT_Role = Bit(3),
    CT_Prop3 = Bit(4)
};

@interface GameViewController () {
    ELWorld *world;
    ELGameObject *tank;
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

@end

@implementation GameViewController

void createCubeGameObject(ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity);

- (void)setupWorld:(ELWorld *)world {
    world->fbWidth = self.view.frame.size.width;
    world->fbHeight = self.view.frame.size.height;
    world->enableDefaultCamera(world->fbWidth / (float)world->fbHeight);
    
    std::string vertexShader = ELFileUtil::stringContentOfShader(ELAssets::shared()->findFile("vtx_phong_es.glsl").c_str());
    std::string fragShader = ELFileUtil::stringContentOfShader(ELAssets::shared()->findFile("frg_phong_es.glsl").c_str());
    std::string shadowFragShader = ELFileUtil::stringContentOfShader(ELAssets::shared()->findFile("frg_shadowmap_es.glsl").c_str());
    std::string waterFragShader = ELFileUtil::stringContentOfShader(ELAssets::shared()->findFile("water_es.glsl").c_str());
    ELEffect * activeEffect = new ELEffect(vertexShader.c_str(), fragShader.c_str());
    ELEffect * shadowEffect = new ELEffect(vertexShader.c_str(), shadowFragShader.c_str());
    ELEffect * waterEffect = new ELEffect(vertexShader.c_str(), waterFragShader.c_str());
    
    activeEffect->identity = "render_scene";
    shadowEffect->identity = "gen_shadow";
    waterEffect->identity = "water";
    world->addNode(activeEffect);
    world->addNode(shadowEffect);
    world->addNode(waterEffect);
    
//    world->addRenderPass(new ELWaterPlaneRenderPass());
//    world->addRenderPass(new ELShadowMapRenderPass());

    activeEffect->frogColor = ELVector4Make(0.2,0.2,0.2,1.0);
    activeEffect->frogStart = 1380;
    activeEffect->frogEnd = 1500;
    
    ELGLState::set(GL_CULL_FACE, GL_TRUE);
    ELGLState::set(GL_DEPTH_TEST, GL_TRUE);

}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [EAGLContext setCurrentContext:self.context];
    
    const char * bundlePath = [[[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/"] UTF8String];
    ELAssets::shared()->addSearchPath(bundlePath);

    world = new ELWorld();
    world->enablePhysics();
    [self setupWorld:world];
    
    ELLight *defaultLight = new ELLight();
    defaultLight->position = ELVector3Make(0,30,30);
    defaultLight->color = ELVector4Make(1.0,1.0,1.0,1.0);
    defaultLight->intensity = 1.0;
    defaultLight->intensityFallOff = 0.0;
    defaultLight->identity = "main-light";
    //    defaultLight->enableShadow();
    world->addNode(defaultLight);
    
    [self createFloor];
    [self createACube];
    [self createArmy];
    
//    ELGameObject *gameObject = new ELGameObject(world);
//    world->addNode(gameObject);
//    gameObject->transform->position = ELVector3Make(0, 3, 0);
//    ELCubeGeometry *cube = new ELCubeGeometry(ELVector3Make(0.1,0.1,0.1));
//    //    gameObject->addComponent(cube);
//    cube->material.diffuse = ELVector4Make(1.0,0.0,0.0,1.0);
//    
//    
//    ELCollisionShape *collisionShape = new ELCollisionShape();
//    collisionShape->asSphere(4);
//    ELRigidBody *rigidBody = new ELRigidBody(collisionShape,1.0);
////    playerRigidBody = rigidBody;
//    rigidBody->collisionGroup = CT_Role;
//    rigidBody->collisionMask = CT_Floor;
//    gameObject->addComponent(rigidBody);
    
//    rigidBody->setVelocity(ELVector3Make(0, 0, 0));
//    world->activedCamera->lockOn(gameObject->transform);
}

- (void)createFloor {
    ELFloat width = 100 * 100;
    ELFloat height = 100 * 100;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,-10,height);
    GLuint diffuseMap,normalMap;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("stone_ground_normal.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("stone_ground.png"))->value;
    
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    createCubeGameObject(world, ELVector3Make(width,1,height),ELVector3Make(0,-0.5,0),0,diffuseMap,normalMap,false, CT_Floor, CT_Prop2 | CT_Prop | CT_Role, ELVector3Make(0,0,0));
}

- (void)createACube {
    ELFloat width = 1;
    ELFloat height = 3;
    ELVector3 floorSize = ELVector3Make(width,3,height);
    GLuint diffuseMap,normalMap;
    normalMap = ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("wood_01.jpg"))->value;
    
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    createCubeGameObject(world, ELVector3Make(width,width,width),ELVector3Make(0,10,0),2,diffuseMap,normalMap,false, CT_Prop, CT_Prop2 | CT_Floor | CT_Role | CT_Prop, ELVector3Make(0,0,0));
}

- (void)createArmy {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap_head,diffuseMap_body,diffuseMap_m4, diffuseMap;
    diffuseMap_head = ELTexture::texture(ELAssets::shared()->findFile("head01.png"))->value;
    diffuseMap_body = ELTexture::texture(ELAssets::shared()->findFile("body01.png"))->value;
    diffuseMap_m4 = ELTexture::texture(ELAssets::shared()->findFile("m4tex_2.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("wood_01.jpg"))->value;
    
    ELGameObject *gameObject = new ELGameObject(world);
    tank = gameObject;
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    //gameObject->transform->scale = ELVector3Make(0.2,0.2,0.2);
//    gameObject->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(M_PI / 2, 0, 1, 0);
    
    //        std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("Airbus A310.fbx").c_str());
    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("walk_1.fbx").c_str());
    ELVector3 finalSize = ELVector3Make(0, 0, 0);
    for (int i = 0; i < geometries.size(); ++i) {
        auto animations = geometries.at(i)->animations;
        auto iter = animations.begin();
        iter ++;
        geometries.at(i)->setAnimation((*iter).second.name);
        geometries.at(i)->generateData();
        gameObject->addComponent(geometries.at(i));
        
        if (finalSize.x < geometries.at(i)->size.x) finalSize.x = geometries.at(i)->size.x;
        if (finalSize.y < geometries.at(i)->size.y) finalSize.y = geometries.at(i)->size.y;
        if (finalSize.z < geometries.at(i)->size.z) finalSize.z = geometries.at(i)->size.z;
    }
    
    finalSize = ELVector3MultiplyScalar(finalSize, 0.005);
    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(finalSize.x / 2, finalSize.y / 2, finalSize.z / 2));
    collisionShape->offset = ELVector3Make(0, -finalSize.y / 2, 0);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape, 100);
    rigidBody->collisionGroup = CT_Prop;
    rigidBody->collisionMask = CT_Floor | CT_Prop | CT_Prop2 | CT_Prop3;
    rigidBody->friction = 0.5;
    gameObject->addComponent(rigidBody);
}

- (void)createTank {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap_head,diffuseMap_body,diffuseMap_m4, diffuseMap;
    diffuseMap_head = ELTexture::texture(ELAssets::shared()->findFile("head01.png"))->value;
    diffuseMap_body = ELTexture::texture(ELAssets::shared()->findFile("body01.png"))->value;
    diffuseMap_m4 = ELTexture::texture(ELAssets::shared()->findFile("m4tex_2.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("wood_01.jpg"))->value;
    
    ELGameObject *gameObject = new ELGameObject(world);
    tank = gameObject;
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    gameObject->transform->scale = ELVector3Make(0.005,0.005,0.005);
    gameObject->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(M_PI / 2, 0, 1, 0);
    
    //        std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("Airbus A310.fbx").c_str());
    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("ausfb.fbx").c_str());
    ELVector3 finalSize = ELVector3Make(0, 0, 0);
    for (int i = 0; i < geometries.size(); ++i) {
        auto animations = geometries.at(i)->animations;
        geometries.at(i)->setAnimation("hello");
        geometries.at(i)->generateData();
        gameObject->addComponent(geometries.at(i));
        
        if (finalSize.x < geometries.at(i)->size.x) finalSize.x = geometries.at(i)->size.x;
        if (finalSize.y < geometries.at(i)->size.y) finalSize.y = geometries.at(i)->size.y;
        if (finalSize.z < geometries.at(i)->size.z) finalSize.z = geometries.at(i)->size.z;
    }
    
    finalSize = ELVector3MultiplyScalar(finalSize, 0.005);
    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(finalSize.x / 2, finalSize.y / 2, finalSize.z / 2));
    collisionShape->offset = ELVector3Make(0, -finalSize.y / 2, 0);
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape, 100);
    rigidBody->collisionGroup = CT_Prop;
    rigidBody->collisionMask = CT_Floor | CT_Prop | CT_Prop2 | CT_Prop3;
    rigidBody->friction = 0.5;
    gameObject->addComponent(rigidBody);
}

- (void)createMonkey {
    ELFloat width = 17 * 3;
    ELFloat height = 17 * 3;
    ELFloat wallHeight = 3.5;
    ELVector3 floorSize = ELVector3Make(width,0.5,height);
    GLuint diffuseMap_head,diffuseMap_body,diffuseMap_m4, diffuseMap;
    diffuseMap_head = ELTexture::texture(ELAssets::shared()->findFile("head01.png"))->value;
    diffuseMap_body = ELTexture::texture(ELAssets::shared()->findFile("body01.png"))->value;
    diffuseMap_m4 = ELTexture::texture(ELAssets::shared()->findFile("m4tex_2.png"))->value;
    diffuseMap = ELTexture::texture(ELAssets::shared()->findFile("wood_01.jpg"))->value;
    
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = ELVector3Make(0,0,0);
    gameObject->transform->scale = ELVector3Make(0.1,0.1,0.1);
    
//        std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("Airbus A310.fbx").c_str());
    std::vector<ELMeshGeometry *> geometries = ELFBXLoader::loadFromFile(ELAssets::shared()->findFile("humanoid.fbx").c_str());
    for (int i = 0; i < geometries.size(); ++i) {
        auto animations = geometries.at(i)->animations;
        geometries.at(i)->setAnimation((*animations.begin()).second.name);
        geometries.at(i)->generateData();
        geometries.at(i)->enableBorder = true;
        geometries.at(i)->borderWidth = 0.2;
        geometries.at(i)->borderColor = ELVector4Make(0.0, 1.0, 0.81, 1.0);
        float sizeY = 0;//geometries.at(i)->size.y;
        gameObject->transform->position = ELVector3Make(0,sizeY * 0.1 / 2,0);
        geometries.at(i)->materials[0].diffuse = ELVector4Make(0.6, 1.0, 1.0, 1.0);
        geometries.at(i)->materials[0].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[0].diffuseMap = diffuseMap;
        geometries.at(i)->materials[1].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[1].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[1].diffuseMap = diffuseMap_body;
        geometries.at(i)->materials[2].diffuse = ELVector4Make(0.0, 0.0, 0.0, 1.0);
        geometries.at(i)->materials[2].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
        geometries.at(i)->materials[2].diffuseMap = diffuseMap_m4;
        gameObject->addComponent(geometries.at(i));
    }
}

void createCubeGameObject(ELWorld *world, ELVector3 size,ELVector3 pos,ELFloat mass,GLuint diffuseMap,GLuint normalMap, bool hasBorder, int collisionGroup, int collisionMask, ELVector3 velocity) {
    
    ELGameObject *gameObject = new ELGameObject(world);
    world->addNode(gameObject);
    gameObject->transform->position = pos;
    ELCubeGeometry *cube = new ELCubeGeometry(size, true);
    gameObject->addComponent(cube);
    cube->materials[0].diffuse = ELVector4Make(0.3, 0.3, 0.3, 1.0);
    cube->materials[0].ambient = ELVector4Make(0.7, 0.7, 0.7, 1.0);
    cube->materials[0].diffuseMap = diffuseMap;//ELTexture::texture(ELAssets::shared()->findFile("rock.png"))->value;
    cube->materials[0].normalMap = normalMap;//ELTexture::texture(ELAssets::shared()->findFile("rock_NRM.png"))->value;
    cube->enableBorder = hasBorder;
    cube->borderWidth = 0.2;
    cube->borderColor = ELVector4Make(1, 0, 0, 1);
    
    ELCollisionShape *collisionShape = new ELCollisionShape();
    collisionShape->asBox(ELVector3Make(size.x / 2, size.y / 2, size.z / 2));
    ELRigidBody *rigidBody = new ELRigidBody(collisionShape, mass);
    rigidBody->collisionGroup = collisionGroup;
    rigidBody->collisionMask = collisionMask;
    rigidBody->friction = 0.5;
    gameObject->addComponent(rigidBody);
    rigidBody->setVelocity(velocity);
}

- (void)dealloc
{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)update {
    static float angle = 0;
    angle += self.timeSinceLastUpdate;
    tank->transform->quaternion = ELQuaternionMakeWithAngleAndAxis(angle, 0, 1, 0);
    world->update(self.timeSinceLastUpdate);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    [view bindDrawable];
    world->render();
}

@end
