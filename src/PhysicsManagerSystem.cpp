#include "ECSCoordinator.hpp"
#include "PhysicsManagerSystem.hpp"


extern ECSCoordinator coordinator;

class MyContactListener : public b2ContactListener {
public:
    virtual void BeginContact(b2Contact* contact) override {
        // This function is called when two fixtures begin to touch

        // cout << "contact detected!" << endl;
    }
    virtual void EndContact(b2Contact* contact) override {
        // This function is called when two fixtures cease to touch

    }
};

void PhysicsManagerSystem::Init(){
    mWorld = new b2World(b2Vec2(0.0f, 0.0f));
    // mContactListener = new MyContactListener();
    // mWorld->SetContactListener(mContactListener);
    mContacts = new vector<pair<Entity, Entity>>;
    mBodyMap = new unordered_map<b2Body*, Entity>;

    SetGravity(0, 98);
    std::cout << "PhysicsManagerSystem initialized!" << std::endl;
}

void PhysicsManagerSystem::Init(float gx, float gy){
    mWorld = new b2World(b2Vec2(gx, gy));
    // mContactListener = new MyContactListener();
    // mWorld->SetContactListener(mContactListener);
    mContacts = new vector<pair<Entity, Entity>>;
    mBodyMap = new unordered_map<b2Body*, Entity>;
    std::cout << "PhysicsManagerSystem initialized!" << std::endl;
}

b2Body* PhysicsManagerSystem::CreateB2Body(Entity entity, BodyType bodyType) {
    b2Body* mBody;
    if (coordinator.HashComponent<Transform>(entity)) {
        if (coordinator.HashComponent<Sprite>(entity)) {
            auto& sprite = coordinator.GetComponent<Sprite>(entity);
            auto& transform = coordinator.GetComponent<Transform>(entity);
            b2BodyDef bodyDef;
            // std::cout << "Mario entity size w: " << sprite.m_Dest.w << ", h:" << sprite.m_Dest.h << std::endl;
            bodyDef.type = (bodyType == BodyType::DYNAMIC_SOLID ? b2_dynamicBody : b2_staticBody);
            bodyDef.position.x = (transform.position.x + 0.5f * sprite.m_Dest.w) * GAME_TO_PHYSICS_SCALE;
            bodyDef.position.y = (transform.position.y + 0.5f * sprite.m_Dest.h) * GAME_TO_PHYSICS_SCALE;
            bodyDef.linearDamping = 0.0f;
            bodyDef.fixedRotation = true;
            // bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&entity);
            mBody = mWorld->CreateBody(&bodyDef);

            b2PolygonShape polygonShape;
            polygonShape.SetAsBox(0.495 * sprite.m_Dest.w * GAME_TO_PHYSICS_SCALE, 0.495 * sprite.m_Dest.h * GAME_TO_PHYSICS_SCALE);
            b2FixtureDef fixtureDef;
            fixtureDef.shape=&polygonShape;
            fixtureDef.friction=0.8f;
            fixtureDef.isSensor = (bodyType == BodyType::STATIC_SENSOR);
            fixtureDef.density = (bodyType == BodyType::DYNAMIC_SOLID) ? 1.0 : 0.0;
            mBody->CreateFixture(&fixtureDef);

            mBodyMap->insert({mBody, entity});
        } else if (coordinator.HashComponent<Tag>(entity)) {
            auto& tag = coordinator.GetComponent<Tag>(entity);
            Tilemap& tilemap = coordinator.m_Tilemap;
            if (tag.theTag == "ground") {
                auto& transform = coordinator.GetComponent<Transform>(entity);
                b2BodyDef bodyDef;
                // std::cout << "Mario entity size w: " << sprite.m_Dest.w << ", h:" << sprite.m_Dest.h << std::endl;
                bodyDef.type = (bodyType == BodyType::DYNAMIC_SOLID ? b2_dynamicBody : b2_staticBody);
                bodyDef.position.x = (transform.position.x + 0.5f * TILE_W * tilemap.m_MapX) * GAME_TO_PHYSICS_SCALE;
                bodyDef.position.y = (transform.position.y + 0.5f * TILE_H * 2) * GAME_TO_PHYSICS_SCALE;
                bodyDef.linearDamping = 0.0f;
                bodyDef.fixedRotation = true;
                // bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&entity);
                mBody = mWorld->CreateBody(&bodyDef);

                b2PolygonShape polygonShape;
                polygonShape.SetAsBox(0.495 * TILE_W * tilemap.m_MapX * GAME_TO_PHYSICS_SCALE, 0.495 * TILE_H * 2 * GAME_TO_PHYSICS_SCALE);
                b2FixtureDef fixtureDef;
                fixtureDef.shape=&polygonShape;
                fixtureDef.friction=0.8f;
                fixtureDef.isSensor = (bodyType == BodyType::STATIC_SENSOR);
                fixtureDef.density = (bodyType == BodyType::DYNAMIC_SOLID) ? 1.0 : 0.0;
                mBody->CreateFixture(&fixtureDef);

                mBodyMap->insert({mBody, entity});   
            } else {
                auto& transform = coordinator.GetComponent<Transform>(entity);
                b2BodyDef bodyDef;
                // std::cout << "Mario entity size w: " << sprite.m_Dest.w << ", h:" << sprite.m_Dest.h << std::endl;
                bodyDef.type = (bodyType == BodyType::DYNAMIC_SOLID ? b2_dynamicBody : b2_staticBody);
                bodyDef.position.x = (transform.position.x + 0.5f * TILE_W) * GAME_TO_PHYSICS_SCALE;
                bodyDef.position.y = (transform.position.y + 0.5f * TILE_H) * GAME_TO_PHYSICS_SCALE;
                bodyDef.linearDamping = 0.0f;
                bodyDef.fixedRotation = true;
                // bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&entity);
                mBody = mWorld->CreateBody(&bodyDef);

                b2PolygonShape polygonShape;
                polygonShape.SetAsBox(0.495 * TILE_W * GAME_TO_PHYSICS_SCALE, 0.495 * TILE_H * GAME_TO_PHYSICS_SCALE);
                b2FixtureDef fixtureDef;
                fixtureDef.shape=&polygonShape;
                fixtureDef.friction=0.8f;
                fixtureDef.isSensor = (bodyType == BodyType::STATIC_SENSOR);
                fixtureDef.density = (bodyType == BodyType::DYNAMIC_SOLID) ? 1.0 : 0.0;
                mBody->CreateFixture(&fixtureDef);

                mBodyMap->insert({mBody, entity}); 
            }   
        }
    }

    return mBody;
}

void PhysicsManagerSystem::RemoveB2Body(Entity entity) {
    if (coordinator.HashComponent<PhysicsBody>(entity)) {
        PhysicsBody& mPhysicsBody = coordinator.GetComponent<PhysicsBody>(entity);
        
        mWorld->DestroyBody(mPhysicsBody.mBody);
        mBodyMap->erase(mPhysicsBody.mBody);
        mPhysicsBody.mBody = nullptr;
        coordinator.getEntityManager().DestroyEntity(entity);
    }
}

void PhysicsManagerSystem::SetB2BodyVx(Entity entity, float vx) {
    if (coordinator.HashComponent<PhysicsBody>(entity)) {
        PhysicsBody mPhysicsBody = coordinator.GetComponent<PhysicsBody>(entity);
        mPhysicsBody.mBody->SetLinearVelocity(b2Vec2(vx * GAME_TO_PHYSICS_SCALE, mPhysicsBody.mBody->GetLinearVelocity().y));
    }
}

void PhysicsManagerSystem::SetB2BodyVy(Entity entity, float vy) {
    if (coordinator.HashComponent<PhysicsBody>(entity)) {
        PhysicsBody mPhysicsBody = coordinator.GetComponent<PhysicsBody>(entity);
        mPhysicsBody.mBody->SetLinearVelocity(b2Vec2(mPhysicsBody.mBody->GetLinearVelocity().x, vy * GAME_TO_PHYSICS_SCALE));
    }
}

void PhysicsManagerSystem::AddB2BodyF(Entity entity, float fx, float fy) {
    if (coordinator.HashComponent<PhysicsBody>(entity)) {
        PhysicsBody mPhysicsBody = coordinator.GetComponent<PhysicsBody>(entity);
        mPhysicsBody.mBody->ApplyForce(b2Vec2(fx * GAME_TO_PHYSICS_SCALE, fy * GAME_TO_PHYSICS_SCALE), mPhysicsBody.mBody->GetPosition(), true);
    }
}

void PhysicsManagerSystem::Update() {
    const float timeStep = 1.0f / 60.0f;
    const int velocityIterations = 6;
    const int positionIterations = 2;

    mWorld->Step(timeStep, velocityIterations, positionIterations);

    // clear contacts list before we check contacts at each step
    mContacts->clear();
    b2Contact *contact = mWorld->GetContactList();
    while(contact){
        if(contact->IsTouching()){
            // cout << "contact happened!" << endl;
            b2Body* bodyA = contact->GetFixtureA()->GetBody();
            b2Body* bodyB = contact->GetFixtureB()->GetBody();
            if(mBodyMap->find(bodyA) != mBodyMap->end() && mBodyMap->find(bodyB) != mBodyMap->end()) {
                Entity e1 = mBodyMap->at(bodyA);
                Entity e2 = mBodyMap->at(bodyB);
                pair<Entity, Entity> p(e1, e2);
                // cout << e1 << endl;
                // cout << e2 << endl;
                mContacts->push_back(p);
            }   
        }
        contact= contact->GetNext();
    }

    PostStep();
}

void PhysicsManagerSystem::PostStep() {
    for(auto& entity : m_Entities) {
        if (coordinator.HashComponent<PhysicsBody>(entity) && coordinator.HashComponent<Transform>(entity) && coordinator.HashComponent<Sprite>(entity)) {
            auto& transform = coordinator.GetComponent<Transform>(entity);
            auto& mPhysicsBody = coordinator.GetComponent<PhysicsBody>(entity);
            auto& sprite = coordinator.GetComponent<Sprite>(entity);
            b2Vec2 position = mPhysicsBody.mBody->GetPosition();
            transform.position.x = position.x / GAME_TO_PHYSICS_SCALE - 0.5f * sprite.m_Dest.w;
            transform.position.y = position.y / GAME_TO_PHYSICS_SCALE - 0.5f * sprite.m_Dest.h;
        } else if (coordinator.HashComponent<PhysicsBody>(entity) && coordinator.HashComponent<Transform>(entity) && coordinator.HashComponent<Tag>(entity)) {
            // auto& tag = coordinator.GetComponent<Tag>(entity);
            auto& transform = coordinator.GetComponent<Transform>(entity);
            auto& mPhysicsBody = coordinator.GetComponent<PhysicsBody>(entity);
            b2Vec2 position = mPhysicsBody.mBody->GetPosition();
            transform.position.x = position.x / GAME_TO_PHYSICS_SCALE - 0.5f * TILE_W;
            transform.position.y = position.y / GAME_TO_PHYSICS_SCALE - 0.5f * TILE_H;
        }
    }
}

void PhysicsManagerSystem::SetGravity(float gx, float gy) {
    if (mWorld != nullptr) {
        mWorld->SetGravity(b2Vec2(gx, gy));
    }
}

vector<pair<Entity, Entity>> *PhysicsManagerSystem::GetContactPairs()
{

    return this->mContacts;
}

void PhysicsManagerSystem::Clear()
{
    int i = 0;
    if (m_Entities.size() > 0)
    {

        for (auto &entity : m_Entities)
        {
            // if (coordinator.GetComponent<Tag>(entity).theTag == "Brick")
            // {
            //     coordinator.getEntityManager().DestroyEntity(entity);
            // }
            coordinator.m_PhysicsManagerSystem->RemoveB2Body(entity);
            // coordinator.getEntityManager().DestroyEntity(entity);
            cout << "Destroying entity: " << entity << endl;
        }
        m_Entities.clear();
    }

    // for (auto it = mBodyMap->begin(); it != mBodyMap->end(); ++it) {
    //     coordinator.m_PhysicsManagerSystem->RemoveB2Body(it->second);
    //     coordinator.getEntityManager().DestroyEntity(it->second);
    // }
    // mBodyMap->clear();

    // for (b2Body* body = mWorld->GetBodyList(); body != nullptr;)
    // {
    //     if(body != nullptr) {
    //         b2Body* nextBody = body->GetNext();
    //         mWorld->DestroyBody(body);
    //         body = nextBody;
    //     } else {
    //         break;
    //     }
    // }
    mBodyMap->clear();
}
