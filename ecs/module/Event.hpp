#pragma once

#include "Common.hpp"
#include <assert.h>
#include <unordered_map>
#include <memory>
#include <string>

using namespace std;

class Event{
public:
    Event() = delete;

    /// @brief declare a constructor
    /// @param eventType see common.hpp for more event types
    explicit Event(EVENT eventType) : m_Event(eventType) {

    }

    EVENT GetEventID() const {
        return m_Event;
    }

    /// @brief use event to pass values
    /// @tparam T the value type you want to pass
    /// @param key what kind of name you want to sign your value
    /// @param value what value do you want to sign
    template<typename T>
    void SetParameter(const string& key, const T& value){
        m_EventMap[key] = make_shared<T>(value);
        // m_EventMap[id] = value;
    }

    /// @brief get value based on the name you signed
    /// @tparam T the value type
    /// @param key your value name
    /// @return the value you want to get
    template <typename T>
    T GetParameter(const string &key) const
    {
        auto it = m_EventMap.find(key);
        if (it == m_EventMap.end())
        {
            std::cerr << "Key not found: " << key << std::endl;
        }
        assert(it != m_EventMap.end());

        return *static_cast<T *>(it->second.get());
    }

    /// @brief convert enum event id to string
    string EventIDToString(EVENT id) const{
        return std::to_string(static_cast<int>(id));
    }

private:
    EVENT                                   m_Event     = {};
    // unordered_map<EventID, any> m_EventMap  = {};
    
    /// @brief stores values related to the name
    unordered_map<string, shared_ptr<void>> m_EventMap  = {};
};