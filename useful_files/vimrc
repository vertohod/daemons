call plug#begin('~/.vim/plugged')
Plug 'scrooloose/nerdtree', {'on': 'NERDTreeToggle'}
Plug 'vim-scripts/bufexplorer.zip'
call plug#end()

syntax on
colo delek

set history=50
set ruler
set showcmd
set nobackup
set tabstop=4
set shiftwidth=4
set smarttab
set expandtab
set nowrap
set noignorecase

set encoding=utf8
set termencoding=utf8
set fileencodings=utf8,koi8-r,cp1251
set fileencoding=utf8
set fileformat=unix
set hls

autocmd BufRead,BufNewFile,BufReadPre [a-z]* set filetype=cpp
autocmd BufRead,BufNewFile,BufReadPre *.txt set filetype=cmake

nmap <C-T> <esc>:NERDTree<cr>
vmap <C-T> <esc>:NERDTree<cr>
imap <C-T> <esc>:NERDTree<cr>

nmap <C-B> <esc>:BufExplorer<cr>
vmap <C-B> <esc>:BufExplorer<cr>
imap <C-B> <esc>:BufExplorer<cr>
