
package.cpath =
    '/home/ssoss/Desktop/Amber/build/thirdparty/lua-5.3.5/tests/lib?.so;'
    .. package.cpath

local mod = require 'my_module'
assert(mod)

print('sin(3)', mod.mysin(3))
print('swap(1, 2)', mod.c_swap(1, 2))
