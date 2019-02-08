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

let &path = '.,/usr/include,/usr/lib/gcc/x86_64-redhat-linux/8/include/'
let ale_c_gcc_options = '-Wall -std=c99 -pedantic -Wvla -Winline'

call AddToInclude('/usr/include/SDL2/')
call AddToInclude('thirdparty/nuklear/include/')
call AddToInclude('thirdparty/glad/include/')
call AddToInclude('thirdparty/stb_image/include/')
call AddToInclude('thirdparty/klib/include/')
call AddToInclude('thirdparty/cmocka/include/')
call AddToInclude('Amber/driver/include/')
call AddToInclude('Amber/ECS/include/')
call AddToInclude('Amber/graphics/include/')
call AddToInclude('Amber/ui/include/')
call AddToInclude('Amber/util/include/')
call AddToInclude('Amber/scripting/include/')
call AddToInclude('build/util/include/')

augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h set filetype=c.doxygen
    autocmd BufRead,BufNewFile *.c set filetype=c.doxygen

    autocmd BufRead,BufNewFile *.h.in ALEDisableBuffer
    autocmd BufRead,BufNewFile *.h.in set filetype=c.doxygen
augroup END

