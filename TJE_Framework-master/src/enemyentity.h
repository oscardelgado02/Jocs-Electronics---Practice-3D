#pragma once

#include "entitymesh.h"
#include "ia.h"

class EnemyEntity : public EntityMesh
{
public:
    //IA
    IA ia;
    int idx;

    //constructor
    EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    //destructor
    ~EnemyEntity();

    //methods overwritten 
    void render();
    void update(float dt);
};

