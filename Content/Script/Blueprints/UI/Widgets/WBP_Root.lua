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
    self:BP_PushTargetWidgetAndBindAttributes(self.LayerWidgetStack_Main, self.MainLayerWidgetClass);
end

function M:BP_PushTargetWidgetAndBindAttributes(InWidgetStack, TargetActivatableWidgetClass)
	-- 参考https://imzlp.com/posts/36659/
    -- 这里注意什么时候使用':'什么时候使用'.'
    -- 通过对象调用函数使用':'
    -- @note self.StaticMeshComponent; self:GetWorld();
	local GameplayWidget = InWidgetStack:BP_AddWidget(TargetActivatableWidgetClass);
	local CommonActivatableWidgetObject = GameplayWidget:Cast(UE.UBlitzCommonActivatableWidget);

	if CommonActivatableWidgetObject ~= nil then
		-- C++中需要使用IxxxInterface::Execute_BindAttributesToViewModels(CommonActivatableWidgetObject, ASC);
		-- 蓝图和UnLua只能这么写
		CommonActivatableWidgetObject:BindAttributesToViewModels_Implementation(
			-- 暂时为0，获取本地玩家
			-- 注意是self不是this!如果看到A null object was passed as a world context object to UEngine::GetWorldFromContextObject().说明写成this了
			UE.UAbilitySystemBlueprintLibrary.GetAbilitySystemComponent(UE.UGameplayStatics.GetPlayerState(self, 0))
		);
	else	-- == nil
		error("Cast failed, please check cast target.");
	end
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
