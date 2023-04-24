#pragma once

#include "System.hpp"
#include <box2d/box2d.h>

class MyContactListener;

class PhysicsManagerSystem : public ISystem
{
    public:
        void Init();
        void Init(float gx, float gy);
        b2Body* CreateB2Body(Entity entity, BodyType bodyType);
        void RemoveB2Body(Entity entity);
        void SetB2BodyVx(Entity entity, float vx);
        void SetB2BodyVy(Entity entity, float vy);
        void AddB2BodyF(Entity entity, float fx, float fy);
        void Update() override;
        void PostStep();
        void SetGravity(float gx, float gy);
        vector<pair<Entity, Entity>>* GetContactPairs();
        // void SetCollision(Entity entity, bool collision);
        // bool GetCollisions(float rx, float ry, float rw, float rh);
        static constexpr float GAME_TO_PHYSICS_SCALE = 0.2f;

        void Clear();

    private:
        b2World* mWorld;
        // MyContactListener* mContactListener;
        vector<pair<Entity, Entity>> *mContacts;
        unordered_map<b2Body*, Entity> *mBodyMap;
};