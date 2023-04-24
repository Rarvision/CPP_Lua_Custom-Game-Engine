#pragma once

#include <array>
#include <unordered_map>
#include <assert.h>

#include "Common.hpp"

using namespace std;

class IComponentArray{
public:
    virtual void ElimiateEntity(Entity entity) = 0;
    virtual ~IComponentArray() = default;
};

/// @brief A component array that stores only one type of the component
template<typename T>
class ComponentArray : public IComponentArray{
public:

    /// @brief Destroy a entity
    /// @param entity entity
    void ElimiateEntity(Entity entity) override{
        auto iter = m_Entity_Index_Map.find(entity);
        if(iter != m_Entity_Index_Map.end()){
            RemoveEntityFromArray(entity);
        }
    }

    /// @brief Add an entity to the component array
    /// @param entity entity
    /// @param componentType componentType
    void AddEntityToArray(Entity entity, T componentType){
        
        assert(m_Entity_Index_Map.find(entity) == m_Entity_Index_Map.end()
                && "Component added to same entity more than once.");

        size_t newIndex = m_CurrentSize;

        m_Entity_Index_Map[entity] = newIndex;
        m_Index_Entity_Map[newIndex] = entity;
        m_ComponentArray[newIndex] = componentType;

        // cout << "ComponentArray AddComponentToArray " << endl;
        // cout << "m_CurrentSize " << m_CurrentSize << endl;
        // cout << "componentType" << componentType;

        m_CurrentSize++;
    }

    /// @brief remove an entity from the array
    /// @param entity entity
    void RemoveEntityFromArray(Entity entity){

        assert(m_Entity_Index_Map.find(entity) != m_Entity_Index_Map.end()
                && "Removing non-existent component.");

        // STEP1:
        // to maintain a good array,
        // 0 1 2 3 4 say we want to remove 2
        // copy final to 2: 0 1 4 3 4

        size_t indexOfRemovedID = m_Entity_Index_Map[entity];
        size_t indexOfLastIndex = m_CurrentSize - 1;
        m_ComponentArray[indexOfRemovedID] = m_ComponentArray[indexOfLastIndex];

        // STEP2:
        // then we just need to switch reference in two maps

        // get the object ID of last element index,
        // then set map to the place where we want to delete
        Entity idOfLastElement = m_Index_Entity_Map[indexOfLastIndex];
        m_Index_Entity_Map[idOfLastElement] = indexOfRemovedID;
        m_Entity_Index_Map[indexOfRemovedID] = idOfLastElement;

        m_Entity_Index_Map.erase(entity);
        m_Index_Entity_Map.erase(idOfLastElement);

        m_CurrentSize--;
    }

    /// @brief get the entity from the component array
    /// @param entity entity
    /// @return entity data
    T& GetComponentData(Entity entity){
        assert(m_Entity_Index_Map.find(entity) != m_Entity_Index_Map.end()
                && "[ComponentArray.GetComponentData] Retrieving non-existent component.");
        
        // ID is in pair with {0-MAX_GameObjectCount}
        // so we use ID to find the pair, and use pair to find the position in m_ComponentArray
        return m_ComponentArray[m_Entity_Index_Map[entity]];
    }

    /// @brief check if the component map has the entity
    /// @param entity entity
    /// @return has the entity or not
    bool HasComponent(Entity entity){
        if(m_Entity_Index_Map.find(entity) != m_Entity_Index_Map.end()){
            return true;
        }
        return false;
    }

private:

    /// @brief array stores a component related to an ID from {0 ~ MAX_GameObject_Count}
    array<T, MAX_EntityCount>           m_ComponentArray {};

    /// @brief Bidirectional Map for ID and index: ID <---> Index
    unordered_map<Entity, size_t>       m_Entity_Index_Map {};
    
    /// @brief Index <---> ID
    unordered_map<size_t, Entity>       m_Index_Entity_Map {};

    /// @brief how many entities are store in the component array
    size_t                              m_CurrentSize {};
};