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
};

