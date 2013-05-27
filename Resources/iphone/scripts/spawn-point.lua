
-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(msg) .. "\n")
    print(debug.traceback())
    print("----------------------------------------")
end

local function main()
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    local cclog = function(...)
        print(string.format(...))
    end
    ---------------
    
    local currentScene = MCSceneContextManager:sharedSceneContextManager():currentContext():getScene()
    local dm = MCDungeonMaster:sharedDungeonMaster()

    dm:setSpawnPointID(currentScene:getScenePackage():getID())
    if not MCGameState:sharedGameState():isSaveFileExists() then
        dm:introduce() -刚开始游戏时游戏介绍
    end
    
end

xpcall(main, __G__TRACKBACK__)
