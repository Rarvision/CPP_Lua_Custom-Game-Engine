#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <typeindex>
#include <assert.h>

#include "ComponentArray.hpp"

using namespace std;

class ComponentManager{
public:
    /// @brief register component function, you must register component first to use the component
    /// @tparam T component type
    template<typename T>
    void RegisterComponent(){
        const char* componentName = typeid(T).name();
        assert(m_component_array_map.find(componentName) == m_component_array_map.end());

        // 1. componentName <---> component ID, componentName <---> ComponentArray pointer
        m_component_type_map[componentName] = componentType++;
        m_component_array_map.insert(make_pair(componentName, make_shared<ComponentArray<T>>()));

        cout << "m_component_type_map key, value are:" << endl;
        for(auto iter = m_component_type_map.begin(); iter != m_component_type_map.end(); iter++){
            cout << "Key: " << iter->first << ", Value: " << iter->second << endl;
        }

        cout << "m_component_array_map key, value are:" << endl;
        for(auto iter = m_component_array_map.begin(); iter != m_component_array_map.end(); iter++){
            cout << "Key: " << iter->first << ", Value: " << iter->second << endl;
        }

        // componentType++;
    }

    /// @brief get the component unique id
    /// @tparam T component type
    /// @return component's id
    template<typename T>
    ComponentType GetComponentTypeUniqueIndex(){
        const char* componentName = typeid(T).name();
        // type_index type = typeid(T);
        assert(m_component_type_map.find(componentName) != m_component_type_map.end());

        return m_component_type_map[componentName];
    }

    /// @brief add a component to an entity
    /// @tparam T component type
    /// @param id entity
    /// @param Component 
    template<typename T>
    void AddComponent(Entity id, T Component){
        GetComponentArray<T>()->AddEntityToArray(id, Component);
    }

    /// @brief remove a component from the entity
    /// @tparam T component type
    /// @param id entity
    template<typename T>
    void RemoveComponent(Entity id){
        GetComponentArray<T>()->RemoveEntityFromArray(id);
    }

    /// @brief destroy an entity, which removes all components within it
    /// @param entity entity
    void DestroyEntity(Entity entity){
        // for all components related to this object, call ElimiateEntity
        for(auto & iter : m_component_array_map){
            auto & component = iter.second;
            component->ElimiateEntity(entity);
        } 
    }

    /// @brief get the component based on the entity
    /// @tparam T component type
    /// @param entity 
    /// @return component
    template<typename T>
    T& GetComponent(Entity entity){
        return GetComponentArray<T>()->GetComponentData(entity);
    }

    /// @brief check if the entity has the component
    /// @tparam T component type
    /// @param entity 
    /// @return has the component or not
    template<typename T>
    bool HasComponent(Entity entity){
        return GetComponentArray<T>()->HasComponent(entity);
    }

private:
    /// @brief A component name ---> a ComponentArray pointer, then add pointer to component array map
    map<const char*, shared_ptr<IComponentArray>>       m_component_array_map;  

    /// @brief A component name ---> A component's ID
    map<const char*, ComponentType>                     m_component_type_map;

    ComponentType componentType {};

    /// @brief get the component array related to the specific component
    /// @tparam T component type
    /// @return a shared_ptr that points to the arary
    template<typename T>
    shared_ptr<ComponentArray<T>> GetComponentArray(){
        const char* componentName = typeid(T).name();
        
        // if we do not find the componentName in m_component_type_map,
        // it means that the component is not registered
        assert(m_component_type_map.find(componentName) != m_component_type_map.end()
                && "Component is not registered here!");

        // use static_pointer_cast to convert IComponentArray to ComponentArray
        return static_pointer_cast<ComponentArray<T>>(m_component_array_map[componentName]);
    }
};