let s:path = expand('<sfile>:p:h') . '/'

function! AddToInclude(dir)
    if a:dir[0] != '/'
        let a:true_dir = s:path . a:dir
    else
        let a:true_dir = a:dir
    endif
    let g:ale_c_gcc_options .= ' -I' . a:true_dir
    let &path .= ',' . a:true_dir
endfunction

let c_gnu = 1
let c_no_c11 = 1

let &path = '.,/usr/include,/usr/lib/gcc/x86_64-redhat-linux/8/include/'
let ale_c_gcc_options = '-Wall -std=c99 -pedantic -Wvla -Winline'

" External Deps
call AddToInclude('/usr/include/SDL2/')
" Internal Deps
call AddToInclude('thirdparty/cglm/include/')
call AddToInclude('thirdparty/glad/include/')
call AddToInclude('thirdparty/lua-5.3/src/')
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
call AddToInclude('build/util/include/')

augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h set filetype=c.doxygen
    autocmd BufRead,BufNewFile *.c set filetype=c.doxygen

    autocmd BufRead,BufNewFile *.h.in ALEDisableBuffer
    autocmd BufRead,BufNewFile *.h.in set filetype=c.doxygen
augroup END

