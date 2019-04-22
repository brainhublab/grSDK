set makeprg=make\ -C\ ./build\ -j8

noremap <F4> :make!<cr>

nnoremap <F5> :!./build/main<cr>
