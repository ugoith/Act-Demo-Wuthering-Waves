--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
---@type BTT_ClearFocus_C
local M = UnLua.Class()
---@param OwnerController AAIController
function M:ReceiveExecuteAI(OwnerController,ControlledPawn)
    OwnerController:K2_ClearFocus()
    self:FinishExecute(true)
end
return M