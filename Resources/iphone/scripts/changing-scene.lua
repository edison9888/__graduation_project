
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

    local sceneController = MCSceneController:sharedSceneController()
    local taskManager = MCTaskManager:sharedTaskManager()
    local shouldChangingScene = true

    -- 检测是否已经接收任务
    if taskManager:isTaskAccepted() then --已经接受任务的情况
        local currentTask = taskManager:getCurrentTask()
        local region = currentTask:getRegion()
        local expectedSceneId = sceneController:getExpectedSceneId()
        --检测请求的场景的ID，是否属于任务地图
        if region:hasScene(expectedSceneId) then --若如属于任务地图，则任务管理器开始任务
            taskManager:startCurrentTask()
        end
    elseif taskManager:isTaskActiviting() then --已经开始任务的情况
        local currentTask = taskManager:getCurrentTask()
        local region = currentTask:getRegion()
        local expectedSceneId = sceneController:getExpectedSceneId()
        if region:hasScene(expectedSceneId) then --若请求的场景为任务地图，则直接加载地图
        else --若请求的场景并非任务地图，则提示是否放弃任务
            local currentScene = MCSceneContextManager:sharedSceneContextManager():currentContext():getScene()
            currentScene:showAbortTaskConfirm("正在任务中，确定要放弃任务？")
            shouldChangingScene = false
        end
    --[[
    else --没有接受任务的情况
         --直接加载地图，不做什么处理
    ]]--
    end

    if shouldChangingScene then
        MCLoading:sharedLoading():loadNewScene()
    end

end

xpcall(main, __G__TRACKBACK__)
