--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BTT_SetFocus_C
local M = UnLua.Class()
---@param OwnerController AAIController
---@param ControlledPawn APawn
function M:ReceiveExecuteAI(OwnerController, ControlledPawn)
    local TargetActor = UE.UBTFunctionLibrary.GetBlackboardValueAsActor(self,self.Target)
    if TargetActor then
        OwnerController:K2_SetFocus(TargetActor)
    end
    self:FinishExecute(true)
end

return M