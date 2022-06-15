#pragma once

#include "entitymesh.h"

class PlayerEntity : public EntityMesh
{
public:
    //constructor
    PlayerEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    //destructor
    ~PlayerEntity();

    //methods overwritten 
    void render();
    void update(float dt);
    void moveFirstPersonCam(Camera* cam, Vector3 position);
    void detectPlayerCollision(Camera* cam, float dt, Vector3 playerVel);
    void detectPlayerCollision2(Camera* cam, float dt, Vector3 playerVel, float speed);
};

