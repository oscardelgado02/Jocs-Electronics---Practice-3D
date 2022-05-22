#pragma once

#include "entitymesh.h"

class WallEntity : public EntityMesh
{
public:
    //constructor
    WallEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    //destructor
    ~WallEntity();

    //methods overwritten 
    void render();
    void update(float dt);
};

