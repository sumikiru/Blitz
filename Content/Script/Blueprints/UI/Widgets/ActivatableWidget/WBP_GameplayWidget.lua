--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_GameplayWidget_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:BP_BindAttributesToViewModels(OwnerASC)
	if UE.UKismetSystemLibrary.DoesImplementInterface(self.VM_HealthPercentBar, UE.UViewModelBindingInterface) then
		-- 错误写法：UE.UViewModelBindingInterface:BindAttributes(self.VM_HealthPercentBar, OwnerASC);
		-- 应该采用蓝图中的写法，如下：（如果是C++，则写为IViewModelBindingInterface::Execute_BindAttributes(VM_HealthPercentBar, OwnerASC)）
		self.VM_HealthPercentBar:BindAttributes_Implementation(OwnerASC);
	else
		error("VM_HealthPercentBar does not implement UViewModelBindingInterface");
	end
end

return M
