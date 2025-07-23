--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_Cube_C

-- UE有自己的文件系统，如果有自定义查找目录的需求，可以修改UnLua.PackagePath来实现。详见官方文档FAQ
-- 不应修改package.path
UnLua.PackagePath = UnLua.PackagePath .. ';Content/Script/FunctionLibrary/?.lua'

local BasicHelper = require("BasicHelper") --只填模块名

local M = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

-- function M:UserConstructionScript()
-- end

function M:ReceiveBeginPlay()
    local msg = [[
    Hello World!
    ]]

    print(msg)
    BasicHelper.PrintToScreen(msg);
end

-- function M:ReceiveEndPlay()
-- end

-- function M:ReceiveTick(DeltaSeconds)
-- end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

return M
