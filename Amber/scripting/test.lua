--- a test module
-- @module test

local M = {}

---
-- @tparam string a First argument
-- @param[out] b Second argument
-- @param[optional] c Third argument
-- @return a
-- @return b
-- @return c
function M.foo2(a, b, c)
    return a, b, c
end

return M
