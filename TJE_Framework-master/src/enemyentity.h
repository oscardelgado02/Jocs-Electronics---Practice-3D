#pragma once

#define JUMPSCARE1 0
#define JUMPSCARE2 1
#define FOOTSTEP_MONSTER 2

#include "entitymesh.h"
#include "ia.h"
#include "sound.h"

class EnemyEntity : public EntityMesh
{
public:
    //IA
    IA ia;
    std::vector<Sound*> sounds;
    bool played_sound;

    //constructor
    EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    //destructor
    ~EnemyEntity();

    //methods overwritten 
    void render();
    void update(float dt);
};

