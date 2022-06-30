#pragma once

#include "entitymesh.h"
#include "sound.h"

class PlayerEntity : public EntityMesh
{
public:
    float cooldown;
    bool cooldown_enable;

    //constructor
    PlayerEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    //destructor
    ~PlayerEntity();

    //methods overwritten 
    void render();
    void update(float dt);
    void movePlayer(Vector3 nextPos);
    float runAndCooldown(float dt, float speed);
    void playSounds(bool status_footsteps);
    Vector3 detectPlayerCollision(float dt, Vector3 playerVel);
    Camera* getPlayerCamera();
};

