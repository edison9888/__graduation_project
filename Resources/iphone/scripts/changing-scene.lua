
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

    -- 检测是否已经接收任务
    if taskManager:isTaskAccepted() then --已经接受任务的情况
        CCLuaLog("isTaskAccepted")
        local currentTask = taskManager:getCurrentTask()
        local region = currentTask:getRegion()
        local expectedSceneId = sceneController:getExpectedSceneId()
        --检测请求的场景的ID，是否属于任务地图
        if region:hasScene(expectedSceneId) then --若如属于任务地图，则任务管理器开始任务
            CCLuaLog("will start current task")
            taskManager:startCurrentTask()
        end
    elseif taskManager:isTaskActiviting() then --已经开始任务的情况
        CCLuaLog("isTaskActiviting")
        local currentTask = taskManager:getCurrentTask()
        local region = currentTask:getRegion()
        local expectedSceneId = sceneController:getExpectedSceneId()
        if region:hasScene(expectedSceneId) then --若请求的场景为任务地图，则直接加载地图
        else --若请求的场景并非任务地图，则提示是否放弃任务
            CCLuaLog("放弃任务？")
        end
    --[[
    else --没有接受任务的情况
         --直接加载地图，不做什么处理
    ]]--
    end
    local loading = MCLoading:sharedLoading()
    --显示加载界面
    loading:show()
    --加载场景
    sceneController:__loadScene()
    --撤销加载界面
    loading:hide()
    --执行切换场景操作
    sceneController:__changeScene()

end

xpcall(main, __G__TRACKBACK__)
