#include "EntityManager.hpp"

EntityManager::EntityManager(){
    for(Entity entity = 0; entity < MAX_EntityCount; entity++){
        m_entityQueue.push(entity);
    }
}

Entity EntityManager::CreateEntity(){
    // assert(m_existedEntityCount < MAX_EntityCount && "Entity number exceeds MAX_GAMEOBJECT");
    // if(m_existedGameObjectCount > MAX_GameObject_Count){
        
    // }
    Entity id = m_entityQueue.front();
    std::cout << "CreateEntity with id" << id << std::endl;
    m_entityQueue.pop();
    m_existedEntityCount++;

    return id;
}

void EntityManager::DestroyEntity(Entity entity){
    assert(entity < MAX_EntityCount && "GAMEOBJECT out of range");

    // Destroy physics body
    // coordinator.m_PhysicsManagerSystem->RemoveB2Body(entity);

    // reset for later use to save id
    m_Signatures[entity].reset();
    m_entityQueue.push(entity);
    m_existedEntityCount--;
}

void EntityManager::SetSignature(Entity entity, Signature signature){
    assert(entity < MAX_EntityCount && "GAMEOBJECT out of range");

    m_Signatures[entity] = signature;

    // TODO:
    // std::cout << " with entity id: " << entity << ", with sigunature" << signature << std::endl;
}

Signature EntityManager::GetSignatureByEntity(Entity entity){
    return m_Signatures[entity];
}