#pragma once

#include "entity.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "input.h"

class EntityMesh : public Entity
{
public:
    //Attributes of this class 
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;

    //constructor
    EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    //destructor
    ~EntityMesh();

    //methods overwritten 
    void render();
    void update(float dt);
    bool checkFrustum();
};