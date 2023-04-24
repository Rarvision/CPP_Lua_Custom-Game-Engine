#pragma once

#include "System.hpp"
#include "Common.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>

using namespace std;

class SystemManager{
public:
    /// @brief register a system (a system cannot be registered twice)
    /// @tparam T system name
    /// @return shared_ptr of the system
    template<typename T>
    shared_ptr<T> RegisterSystem(){
        const char* componentName = typeid(T).name();

        assert(m_System.find(componentName) == m_System.end() 
                && "Registering system more than once.");

        // create a shared_ptr to the system and points to it, also add to map
        auto system = make_shared<T>();
        m_System[componentName] = system;

        return system;
    }

    /// @brief set a signature for a system
    /// @tparam T system name
    /// @param signature [compnent signature] what kind of components the system cares about
    template<typename T>
    void SetSignature(Signature signature){
        const char* componentName = typeid(T).name();

        assert(m_System.find(componentName) != m_System.end() 
                && "Registering system more than once.");

        // cout << "Set System Signature " << typeid(T).name() << " set signature: " << signature << endl;
    
        m_Signature[componentName] = signature;
    }

    /// @brief remove an entity from the system
    /// @param entity 
    void DestroyEntity(Entity entity){
        for(auto const& mapPairs : m_System){
            auto const& system = mapPairs.second;
            system->m_Entities.erase(entity);
        }
    }

    /// @brief when an entity has or removes a component, the system may add the entity to the m_Entity, or remove it
    /// @param entity 
    /// @param entitySignature 
    void EntitySignatureChanged(Entity entity, Signature entitySignature){
        for(auto const& mapPairs : m_System){
            auto const& componenName    = mapPairs.first;
            auto const& system          = mapPairs.second;
            auto const& systemSignature = m_Signature[componenName];

            if((entitySignature & systemSignature) == systemSignature){
                system->m_Entities.insert(entity);
            }
            else{
                system->m_Entities.erase(entity);
            }
        }
    }

private:
    /// @brief system name <---> signature
    unordered_map<const char*, Signature>           m_Signature {};

    /// @brief system name <---> specific system shared_ptr
    //  Since ISystem is a base class where all sub system inherits from it,
    //  it's the same relationship as IComponent and ComponentArray
    unordered_map<const char*, shared_ptr<ISystem>>  m_System {};
};