let s:path = expand('<sfile>:p:h') . '/'

function! AddToInclude(dir)
    let l:true_dir = s:path . a:dir
    let g:ale_c_gcc_options .= ' -I' . l:true_dir
    let &path .= ',' . l:true_dir
endfunction

" We are using standard c99 - don't highlight mistakes
let c_gnu = 1
let c_no_c11 = 1

" The default path - this is local to my machine
let &path = '.,/usr/include,/usr/lib/gcc/x86_64-redhat-linux/8/include/,/usr/include/SDL2/,/usr/local/include/'
let ale_c_gcc_options = '-Wall -std=c90 -Wvla -Winline -I/usr/include/SDL2/ -I/usr/local/include'

" Internal Deps
call AddToInclude('thirdparty/cmocka/include/')
call AddToInclude('thirdparty/cglm/include/')
call AddToInclude('thirdparty/glad/include/')
call AddToInclude('thirdparty/lua-5.3/include/')
call AddToInclude('thirdparty/nuklear/include/')
call AddToInclude('thirdparty/sqlite3/include/')
call AddToInclude('thirdparty/stb/include/')
" Amber modules
call AddToInclude('Amber/driver/include/')
call AddToInclude('Amber/ECS/include/')
call AddToInclude('Amber/graphics/include/')
call AddToInclude('Amber/scripting/include/')
call AddToInclude('Amber/ui/include/')
call AddToInclude('Amber/util/include/')
call AddToInclude('Amber/render/include/')
call AddToInclude('_build/util/include/')

augroup project
    autocmd!
    " Was annoying due to how syntax highlighting works
    autocmd BufRead,BufNewFile *.h set filetype=c
    autocmd BufRead,BufNewFile *.c set filetype=c

    autocmd BufRead,BufNewFile *.h.in ALEDisableBuffer
    autocmd BufRead,BufNewFile *.h.in set filetype=c.doxygen
augroup END

