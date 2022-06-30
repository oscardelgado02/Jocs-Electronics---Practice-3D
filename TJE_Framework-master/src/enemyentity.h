#pragma once

#include "animatedentity.h"
#include "ia.h"
#include "sound.h"
#include "world.h"
#include "animation.h"

#define ANIM_IDLE 0
#define ANIM_WALK 1
#define ANIM_JUMPSCARE 2

class EnemyEntity : public AnimatedEntity
{
public:
    bool played_sound;
    Vector3 target_player;
    float yaw;

    //constructor
    EnemyEntity(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color, Mesh* animated_mesh, Animation* idle_animation);
    //destructor
    ~EnemyEntity();

    //methods overwritten 
    void render();
    void update(float dt);
    float distanceToPlayer();
    void changeEnemyAnimation();
    void setTargetPlayer(Vector3 target);
    void playSounds();
    void movementAndRotation(float dt, float speed);
};

