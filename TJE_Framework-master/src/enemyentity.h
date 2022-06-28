#pragma once

#define JUMPSCARE1 0
#define JUMPSCARE2 1
#define FOOTSTEP_MONSTER 2

#include "entitymesh.h"
#include "ia.h"
#include "sound.h"
#include "world.h"

class EnemyEntity : public EntityMesh
{
public:
    //IA
    IA ia;
    std::vector<Sound*> sounds;
    bool played_sound;
    Vector3 target_player;
    Vector3 last_direction;

    //constructor
    EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    //destructor
    ~EnemyEntity();

    //methods overwritten 
    void render();
    void update(float dt);
    float distanceToPlayer();
    void setTargetPlayer(Vector3 target);
    void playSounds();
    void movementAndRotation(float dt, float speed);
};

