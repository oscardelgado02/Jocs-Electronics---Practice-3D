#pragma once

#include "includes.h"
#include "framework.h"

class Entity
{
public:
    Entity(std::string name, Matrix44 model); //constructor
    virtual ~Entity() {}; //destructor

    //some attributes 
    std::string name;
    Matrix44 model;

    //methods overwritten by derived classes 
    virtual void render() {};
    virtual void update(float elapsed_time) {};

    //some useful methods...
    Vector3 getPosition();
    void setModel(Matrix44 model);
};

