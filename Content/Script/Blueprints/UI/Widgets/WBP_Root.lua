--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Root_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    -- 参考https://imzlp.com/posts/36659/
    -- 这里注意什么时候使用':'什么时候使用'.'
    -- 通过对象调用函数使用':'
    -- @note self.StaticMeshComponent; self:GetWorld();
    self.LayerWidgetStack_Main:BP_AddWidget(self.MainLayerWidgetClass);
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
