
-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(msg) .. "\n")
    print(debug.traceback())
    print("----------------------------------------")
end

MCSceneRegion = 
{ 
    MCVillage = 1, 
    MCCity    = 2, 
    MCForest  = 4, 
    MCDesert  = 8, 
    MCGlacier = 16, 
    MCDungeon = 32, 
}

local function main()
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    local cclog = function(...)
        print(string.format(...))
    end
    ---------------

    if MCFlagManager:sharedFlagManager():isAreaBlocked() then
        MCDungeonMaster:sharedDungeonMaster():speak('高级任务没有解锁，请先完成低级任务！')
    else
        local issuingTaskUI = MCIssuingTaskUI:create()
        --lua坑爹了
        issuingTaskUI:setSceneRegion(MCSceneRegion.MCGlacier + MCSceneRegion.MCDungeon)
        local currentScene = MCSceneContextManager:sharedSceneContextManager():currentContext():getScene()
        issuingTaskUI:attach(currentScene)
    end    
end

xpcall(main, __G__TRACKBACK__)
