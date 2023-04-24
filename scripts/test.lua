local test = {}

local input
local space_input
local speed
local jump_speed
local is_jumping
local current_entity
local collisionTable

function test.start()
    -- print("[Lua] test2.start called")
    current_entity = engine.get_entity()
    -- print(currentEntity)
    speed = 200000.0
    jump_speed = 2000000.0
    is_jumping = false
    engine.add_b2_body("dynamic_solid")
end

function test.update()
    -- jump_speed = 2000000.0
    input = engine.get_horizontal_input()
    engine.add_force(speed*input, 0)
    -- engine.set_velocity()
    space_input = -engine.get_space_input()
    -- print("space_input: "..space_input)
    if not is_jumping and space_input ~= 0 then
        jump()
    end
    
    collisionTable = remove_duplicates(engine.on_collision(current_entity))
    if next(collisionTable) ~= nil then
        for _, value in pairs(collisionTable) do
            -- print(value)
            if engine.get_tag_component(value) == "Brick" then
                is_jumping = false
            end
            if engine.get_tag_component(value) == "Coin" then
                engine.destroy_entity(value)
            end
            if engine.get_tag_component(value) == "Trophy" then
                engine.win()
            end
        end
    end
end

function jump()
    print("jump called")
    is_jumping = true
    engine.add_force(speed * input, jump_speed * space_input)
end

function remove_duplicates(tbl)
    local unique_tbl = {}
    local dict = {}
    
    for _, value in ipairs(tbl) do
        if not dict[value] then
            table.insert(unique_tbl, value)
            dict[value] = true
        end
    end

    return unique_tbl
end


return test
