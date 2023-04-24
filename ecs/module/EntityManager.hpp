#pragma once

#include <iostream>
#include <queue>
#include <array>
#include <assert.h>

#include "Common.hpp"


using namespace std;

/// @brief manage all entities
// each entity has a signature
class EntityManager{
public:

    /// @brief add all entities to the queue
    EntityManager();

    /// @brief create an entity and give it an id
    /// @return an entity with id
    Entity CreateEntity();

    /// @brief destroy an entity
    /// @param entity 
    void DestroyEntity(Entity entity);

    /// @brief give entity an signature(signature is used to record what entity has which component)
    /// @param entity 
    /// @param signature 
    void SetSignature(Entity entity, Signature signature);

    /// @brief get an entity's signature
    /// @param entity 
    /// @return Signature
    Signature GetSignatureByEntity(Entity entity);

private:
    
    uint32_t                           m_existedEntityCount {};

    queue<Entity>                      m_entityQueue {};

    array<Signature, MAX_EntityCount>  m_Signatures {};
};
