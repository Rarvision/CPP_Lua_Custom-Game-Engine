#pragma once

#include "Event.hpp"
#include <unordered_map>
#include <list>
#include <functional>
#include <vector>

using namespace std;

class EventManager{
public:
    using EventCallback = function<void(Event& event)>;

    /// @brief add a listener for an event
    /// @param eventId EVENT from Common.hpp
    /// @param callBack lambda function
    void AddListener(EVENT eventId, const EventCallback& callBack){
        m_EventMap[eventId].push_back(callBack);
    }   

    /// @brief remove a listener for an event
    /// @param eventId EVENT from Common.hpp
    /// @param callBack lambda function
    void RemoveListener(EVENT eventId, const EventCallback& callBack){
        auto& callBackFuncVector = m_EventMap[eventId];
        for (auto iterator = callBackFuncVector.begin(); iterator != callBackFuncVector.end(); ++iterator)
        {
            if (iterator->target_type() == callBack.target_type())
            {
                callBackFuncVector.erase(iterator);
                break;
            }
        }
    }

    /// @brief trigger events based on EVENT ID
    /// @param id EVENT from Common.hpp
    void Trigger(EVENT id){
        Event event(id);

        for(auto const& callBack : m_EventMap[id]){
            callBack(event);
        }
    }

    void Trigger(Event& event){
        auto eventId = event.GetEventID();

        for(auto const& callBack : m_EventMap[eventId]){
            callBack(event);
        }
    }

private:
    /// @brief a map that stores an event and data that need to be passed by this event
    unordered_map<EVENT, vector<EventCallback>> m_EventMap = {};
};