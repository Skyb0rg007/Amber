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

let &path = '.,/usr/include'
let ale_c_gcc_options = '-Wall -std=c11 -pedantic -Wvla -Winline -Wconversion'

call AddToInclude('/usr/include/SDL2/')
call AddToInclude('thirdparty/nuklear/include/')
call AddToInclude('thirdparty/glad/include/')
call AddToInclude('thirdparty/stb_image/include/')
call AddToInclude('thirdparty/klib/include/')
call AddToInclude('Amber/driver/include/')
call AddToInclude('Amber/ECS/include/')
call AddToInclude('Amber/graphics/include/')
call AddToInclude('Amber/ui/include/')
call AddToInclude('Amber/util/include/')

augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h set filetype=c.doxygen
    autocmd BufRead,BufNewFile *.c set filetype=c.doxygen

    autocmd BufRead,BufNewFile *.h.in ALEDisableBuffer
    autocmd BufRead,BufNewFile *.h.in set filetype=c.doxygen
augroup END

