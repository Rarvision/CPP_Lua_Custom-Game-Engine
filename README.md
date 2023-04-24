## Introduction

This custom engine is based on the Entity-Component-System (ECS) design pattern. This widely-recognized architectural approach emphasizes the separation of concerns and offers a modular, efficient framework for organizing game objects and their associated behaviors. A dedicated ECS Coordinator is responsible for registering, managing, and orchestrating the interactions among entities, components, and systems within our game engine.

Our game engine leverages the strengths of the Entity-Component-System design pattern to provide a robust, flexible, and high-performing foundation for developing diverse and engaging gaming experiences. Integrating a dedicated ECS Coordinator ensures seamless management and coordination of all entities, components, and systems, streamlining development efforts and promoting efficient collaboration among team members.

Our game engine provides an editor GUI allowing users to edit the map and set up levels for 2D platform games. In the editor, the user can freely place any provided game tiles, such as bricks or coins, at any location on the screen. The user-defined level will be saved and reflected in the actual gameplay.

Contributors: Xueqing Wang, Mao Zhang, Zun Wang, Huan-Feng Yeh

## Basic Elements

### Entity

Entities are the game objects in the game world, such as characters, coins, or bricks in our Mario game. They are containers that associate various components with creating a unique game object.

### Component

Components are data containers that store the attributes and properties of an entity. They represent different aspects of an entity, such as transform/position, physics body, sprite, or tag. Components are designed to be modular and reusable, meaning that they can be added or removed from entities as needed. Developers can create various game objects by just combining different components. For example, the Transform component manages the position of any entity, the Sprite component represents the sprite of a game object for rendering, and the PhysicsBody component sets the 2D body for a game object in the physics world of a game.

### System

Systems are responsible for the logic and functionality of the game. They operate on entities with specific components, updating their state and implementing game mechanics. Systems are usually organized around specific functionality, such as rendering, game control, physics, or gameplay logic(scripting). By separating the game logic from the data, systems can process all relevant entities and components consistently and efficiently. In our game engine, the scripting system allows the user to write gameplay in Lua without knowing the details behind the game engine API.

## Scripting

The scripting system of this game engine allows developers to write game logic using Lua scripts. This document provides an overview of how the scripting system works, and how to use it. 

##  Script lifecycle

Every script file will be exported as a package and loaded into the game engine. All script files share the same Lua stack and will remain until the shutdown of the scripting system unless manually removed. For exportation, every Lua file must define and return one local table. Users can expose their custom variables and functions as "public" by registering them to the file's local table.

The scripting system defines two default methods, `start()` and `update()`for every Lua script. These methods are called by the engine automatically, and you can implement them in your scripts to handle game logic and events. These two functions must be registered to the file's local table to be exposed to the engine.

### start()

The `start()` method is called when the Lua script is first loaded into the game engine. This method is ideal for initializing variables, setting up objects, or performing any one-time setup tasks required for the script. The `start()` method is called only once per script during its lifecycle.
```lua
function file.start()
    print("Script initialized")
end
```
### update()

The `update()` method is called once per frame during the game's runtime. This is where you can implement logic that needs to be executed every frame, such as moving objects, checking for collisions, or handling user input. The `update()` method is called continuously throughout the script's lifecycle.
```lua
function file.update()
    print("Called once per frame")
end
```

## APIs

You can access the bindings provided by the game engine to manipulate entities and components. Since all custom bindings are stored in a global table called `engine`, you must use the `engine.` prefix before the binding function name. Here are some examples:
```lua
-- Create a new entity and set its name as "Player"
local entity = engine.create_entity()
engine.set_name_component(entity, "Player")
```

### create_entity

`function create_entity() -> entity` 

C++ Binding: `Binding::lua_CreateEntity`

Create a new entity and return its ID.

### destroy_entity

`function destroy_entity(entity) -> boolean` 

C++ Binding: `Binding::lua_DestroyEntity`

Destroy an entity by its ID. Returns `true` if successful, `false` otherwise.

### get_entity

`function get_entity() -> entity` 

C++ Binding: `Binding::lua_GetEntity`

Get the current entity ID.

### get_entity_by_name

`function get_entity_by_name(name) -> entity` 

C++ Binding: `Binding::lua_GetEntityByName`

Find and return an entity ID based on the given name.

### get_transform_component

`function get_transform_component(entity) -> transform` 

C++ Binding: `Binding::lua_GetTransformComponent`

Get the transform component of the given entity. If the entity ID is not provided, use the current entity.

Position can be accessed like:
```lua
local trans = engine.get_transform_component(entity);
local pos = trans2.position
print("pos.x: "..pos.x.." pos.y: "..pos.y)
```


### set_name_component

`function set_name_component(entity, name) -> boolean` 

C++ Binding: `Binding::lua_SetNameComponent`

Set the name component of the given entity. If the entity ID is not provided, use the current entity. Returns `true` if successful, `false` otherwise.

### remove_name_component

`function remove_name_component(entity) -> boolean` 

C++ Binding: `Binding::lua_RemoveNameComponent`

Remove the name component of the given entity. If the entity ID is not provided, use the current entity. Returns `true` if successful, `false` otherwise.

### get_tag_component

`function get_tag_component(entity, tag) -> string`

C++ Binding:: Binding::lua_GetTagComponent

Get the tag of the given entity and return the tag name.  If the entity ID is not provided, use the current entity.

### set_tag_component

`function set_tag_component(entity, tag) -> boolean` 

C++ Binding: `Binding::lua_SetTagComponent`

Set the tag component of the given entity. If the entity ID is not provided, use the current entity. Returns `true` if successful, `false` otherwise.

### remove_tag_component

`function remove_tag_component(entity) -> boolean` 

C++ Binding: `Binding::lua_RemoveTagComponent`

Remove the tag component of the given entity. If the entity ID is not provided, use the current entity. Returns `true` if successful, `false` otherwise.

### get_script_component

`function get_script_component(entity) -> string` 

C++ Binding: `Binding::lua_GetScriptComponent`

Get the script component of the given entity and return the script name.

### add_b2_body

`function add_b2_body(entity, bodyDef) -> body` 

C++ Binding: `Binding::lua_AddB2Body`

Add a Box2D body to the given entity with the provided body definition.

### remove_b2_body

`function remove_b2_body(entity) -> boolean` 

C++ Binding: `Binding::lua_RemoveB2Body`

Remove the Box2D body from the given entity. Returns `true` if successful, `false` otherwise.

### set_velocity

`function set_velocity(entity, x, y) -> boolean` 

C++ Binding: `Binding::lua_SetVelocity`

Set the velocity of the given entity in the x and y directions. Returns `true` if successful, `false` otherwise.

### add_force

`function add_force(entity, x, y) -> boolean` 

C++ Binding: `Binding::lua_AddForce`

Add force to the given entity in the x and y directions. Returns `true` if successful, `false` otherwise.

### check_collision

`function check_collision(entity_a, entity_b) -> boolean` 

C++ Binding: `Binding::lua_CheckCollision`

Checks if two specified entities are currently colliding. Returns `true` if the two entities are colliding, `false` otherwise.

### on_collision

`function on_collision(entity) -> table` 

C++ Binding:: `Binding::lua_OnCollision`

Retrieves a list of all entities that the specified entity is currently colliding with.  Returns a table containing the IDs of all colliding entities.

### get_horizontal_input

`function get_horizontal_input() -> number` 

C++ Binding: `Binding::lua_GetHorizontalInput`

Get the current horizontal input as a number. Positive values indicate movement to the right, and negative values indicate movement to the left.

### get_vertical_input

`function get_vertical_input() -> number` 

C++ Binding: `Binding::lua_GetVerticalInput`

Get the current vertical input as a number. Positive values indicate movement upward, and negative values indicate movement downward.

## Example Script

An example Lua script using the custom game engine bindings might look like this:
```lua
example = {}

local entity = engine.create_entity()
engine.set_name_component(entity, "Player")

function example.start()
    -- Initialize the player entity
    engine.set_tag_component(entity, "Player")
end

function example.update()
    -- Update the player entity
    local x = engine.get_horizontal_input()
    local y = engine.get_vertical_input()
    engine.set_velocity(entity, x, y)
end

return 1
```

This script creates a player entity, sets its name to "Player", initializes the entity with the "Player" tag, and updates the entity's velocity based on input.

