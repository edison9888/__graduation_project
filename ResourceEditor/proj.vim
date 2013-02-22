let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <silent> <SNR>46_yrrecord =YRRecord3()
imap <D-BS> 
imap <M-BS> 
imap <M-Down> }
inoremap <D-Down> <C-End>
imap <M-Up> {
inoremap <D-Up> <C-Home>
noremap! <M-Right> <C-Right>
noremap! <D-Right> <End>
noremap! <M-Left> <C-Left>
noremap! <D-Left> <Home>
inoremap <Plug>ZenCodingAnchorizeSummary :call zencoding#anchorizeURL(1)a
inoremap <Plug>ZenCodingAnchorizeURL :call zencoding#anchorizeURL(0)a
inoremap <Plug>ZenCodingRemoveTag :call zencoding#removeTag()a
inoremap <Plug>ZenCodingSplitJoinTagInsert :call zencoding#splitJoinTag()
inoremap <Plug>ZenCodingToggleComment :call zencoding#toggleComment()a
inoremap <Plug>ZenCodingImageSize :call zencoding#imageSize()a
inoremap <Plug>ZenCodingPrev :call zencoding#moveNextPrev(1)
inoremap <Plug>ZenCodingNext :call zencoding#moveNextPrev(0)
inoremap <Plug>ZenCodingBalanceTagOutwardInsert :call zencoding#balanceTag(-1)
inoremap <Plug>ZenCodingBalanceTagInwardInsert :call zencoding#balanceTag(1)
inoremap <Plug>ZenCodingExpandWord u:call zencoding#expandAbbr(1,"")a
inoremap <Plug>ZenCodingExpandAbbr u:call zencoding#expandAbbr(0,"")a
inoremap <silent> <SNR>47_yrrecord =YRRecord3()
imap <silent> <Plug>IMAP_JumpBack =IMAP_Jumpfunc('b', 0)
imap <silent> <Plug>IMAP_JumpForward =IMAP_Jumpfunc('', 0)
inoremap <silent> <S-Tab> =snipMate#BackwardsSnippet()
inoremap <C-Tab> 	
map  :set guifont=Osaka-Mono:h
nmap  h|
xnoremap 	 s:let g:snipmate_content_visual=getreg( 1 )
snoremap <silent> 	 i<Right>=snipMate#TriggerSnippet()
nmap <NL> j_
vmap <NL> <Plug>IMAP_JumpForward
omap <NL> j_
map  k_
nmap  l|
nnoremap <silent>  :YRReplace '1', 'p'
nnoremap <silent>  :YRReplace '-1', 'P'
map  :tabnext
map  :tabprev
map  :tabclose 
map  :tabnew
noremap 5 yypVr^
noremap 4 yypVr-
noremap 3 yypVr=
noremap 2 yyPVr*yyjp
noremap 1 yyPVr#yyjp
snoremap  b<BS>
vmap c <Plug>ZenCodingCodePretty
nmap A <Plug>ZenCodingAnchorizeSummary
nmap a <Plug>ZenCodingAnchorizeURL
nmap k <Plug>ZenCodingRemoveTag
nmap j <Plug>ZenCodingSplitJoinTagNormal
vmap m <Plug>ZenCodingMergeLines
nmap / <Plug>ZenCodingToggleComment
nmap i <Plug>ZenCodingImageSize
nmap N <Plug>ZenCodingPrev
nmap n <Plug>ZenCodingNext
vmap D <Plug>ZenCodingBalanceTagOutwardVisual
nmap D <Plug>ZenCodingBalanceTagOutwardNormal
vmap d <Plug>ZenCodingBalanceTagInwardVisual
nmap d <Plug>ZenCodingBalanceTagInwardNormal
nmap , <Plug>ZenCodingExpandNormal
vmap , <Plug>ZenCodingExpandVisual
map  :po
snoremap % b<BS>%
snoremap ' b<BS>'
smap ,cc :botright cope 
nmap ,ca <Plug>NERDCommenterAltDelims
xmap ,cu <Plug>NERDCommenterUncomment
nmap ,cu <Plug>NERDCommenterUncomment
xmap ,cb <Plug>NERDCommenterAlignBoth
nmap ,cb <Plug>NERDCommenterAlignBoth
xmap ,cl <Plug>NERDCommenterAlignLeft
nmap ,cl <Plug>NERDCommenterAlignLeft
nmap ,cA <Plug>NERDCommenterAppend
xmap ,cy <Plug>NERDCommenterYank
nmap ,cy <Plug>NERDCommenterYank
xmap ,cs <Plug>NERDCommenterSexy
nmap ,cs <Plug>NERDCommenterSexy
xmap ,ci <Plug>NERDCommenterInvert
nmap ,ci <Plug>NERDCommenterInvert
nmap ,c$ <Plug>NERDCommenterToEOL
xmap ,cn <Plug>NERDCommenterNested
nmap ,cn <Plug>NERDCommenterNested
xmap ,cm <Plug>NERDCommenterMinimal
nmap ,cm <Plug>NERDCommenterMinimal
xmap ,c  <Plug>NERDCommenterToggle
nmap ,c  <Plug>NERDCommenterToggle
xmap ,cc <Plug>NERDCommenterComment
nmap ,cc <Plug>NERDCommenterComment
vnoremap <silent> ,,w :call EasyMotion#WB(1, 0)
onoremap <silent> ,,w :call EasyMotion#WB(0, 0)
nnoremap <silent> ,,w :call EasyMotion#WB(0, 0)
vnoremap <silent> ,,t :call EasyMotion#T(1, 0)
onoremap <silent> ,,t :call EasyMotion#T(0, 0)
nnoremap <silent> ,,t :call EasyMotion#T(0, 0)
vnoremap <silent> ,,n :call EasyMotion#Search(1, 0)
onoremap <silent> ,,n :call EasyMotion#Search(0, 0)
nnoremap <silent> ,,n :call EasyMotion#Search(0, 0)
vnoremap <silent> ,,k :call EasyMotion#JK(1, 1)
onoremap <silent> ,,k :call EasyMotion#JK(0, 1)
nnoremap <silent> ,,k :call EasyMotion#JK(0, 1)
vnoremap <silent> ,,j :call EasyMotion#JK(1, 0)
onoremap <silent> ,,j :call EasyMotion#JK(0, 0)
nnoremap <silent> ,,j :call EasyMotion#JK(0, 0)
vnoremap <silent> ,,gE :call EasyMotion#EW(1, 1)
onoremap <silent> ,,gE :call EasyMotion#EW(0, 1)
nnoremap <silent> ,,gE :call EasyMotion#EW(0, 1)
vnoremap <silent> ,,f :call EasyMotion#F(1, 0)
onoremap <silent> ,,f :call EasyMotion#F(0, 0)
nnoremap <silent> ,,f :call EasyMotion#F(0, 0)
vnoremap <silent> ,,e :call EasyMotion#E(1, 0)
onoremap <silent> ,,e :call EasyMotion#E(0, 0)
nnoremap <silent> ,,e :call EasyMotion#E(0, 0)
vnoremap <silent> ,,b :call EasyMotion#WB(1, 1)
onoremap <silent> ,,b :call EasyMotion#WB(0, 1)
nnoremap <silent> ,,b :call EasyMotion#WB(0, 1)
vnoremap <silent> ,,W :call EasyMotion#WBW(1, 0)
onoremap <silent> ,,W :call EasyMotion#WBW(0, 0)
nnoremap <silent> ,,W :call EasyMotion#WBW(0, 0)
vnoremap <silent> ,,T :call EasyMotion#T(1, 1)
onoremap <silent> ,,T :call EasyMotion#T(0, 1)
nnoremap <silent> ,,T :call EasyMotion#T(0, 1)
vnoremap <silent> ,,N :call EasyMotion#Search(1, 1)
onoremap <silent> ,,N :call EasyMotion#Search(0, 1)
nnoremap <silent> ,,N :call EasyMotion#Search(0, 1)
vnoremap <silent> ,,ge :call EasyMotion#E(1, 1)
onoremap <silent> ,,ge :call EasyMotion#E(0, 1)
nnoremap <silent> ,,ge :call EasyMotion#E(0, 1)
vnoremap <silent> ,,F :call EasyMotion#F(1, 1)
onoremap <silent> ,,F :call EasyMotion#F(0, 1)
nnoremap <silent> ,,F :call EasyMotion#F(0, 1)
vnoremap <silent> ,,E :call EasyMotion#EW(1, 0)
onoremap <silent> ,,E :call EasyMotion#EW(0, 0)
nnoremap <silent> ,,E :call EasyMotion#EW(0, 0)
vnoremap <silent> ,,B :call EasyMotion#WBW(1, 1)
onoremap <silent> ,,B :call EasyMotion#WBW(0, 1)
nnoremap <silent> ,,B :call EasyMotion#WBW(0, 1)
nnoremap <silent> ,b :CommandTBuffer
nnoremap <silent> ,t :CommandT
map ,g :call IncludeGuard()
nmap ,p :set paste!|set paste?
nmap ,/ :nohl
omap ,cc :botright cope 
map ,r :call Replace()
vnoremap < <gv
vnoremap > >gv
xnoremap <silent> P :YRPaste 'P', 'v'
nnoremap <silent> P :YRPaste 'P'
xmap S <Plug>VSurround
snoremap U b<BS>U
vmap [% [%m'gv``
snoremap \ b<BS>\
vmap ]% ]%m'gv``
snoremap ^ b<BS>^
snoremap ` b<BS>`
vmap a% [%v]%
nmap cs <Plug>Csurround
xnoremap <silent> d :YRDeleteRange 'v'
nmap ds <Plug>Dsurround
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> gp :YRPaste 'gp'
nnoremap <silent> gP :YRPaste 'gP'
xmap gS <Plug>VgSurround
xnoremap <silent> p :YRPaste 'p', 'v'
nnoremap <silent> p :YRPaste 'p'
xnoremap <silent> s :echoerr 'surround.vim: Visual mode s has been removed in favor of S'
xnoremap <silent> x :YRDeleteRange 'v'
xnoremap <silent> y :YRYankRange 'v'
nmap ySS <Plug>YSsurround
nmap ySs <Plug>YSsurround
nmap yss <Plug>Yssurround
nmap yS <Plug>YSurround
nmap ys <Plug>Ysurround
nnoremap <silent> <SNR>46_yrrecord :call YRRecord3()
map <D-t> :CommandT
map <M-Down> }
noremap <D-Down> <C-End>
map <M-Up> {
noremap <D-Up> <C-Home>
noremap <M-Right> <C-Right>
noremap <D-Right> <End>
noremap <M-Left> <C-Left>
noremap <D-Left> <Home>
snoremap <Left> bi
snoremap <Right> a
snoremap <BS> b<BS>
snoremap <silent> <S-Tab> i<Right>=snipMate#BackwardsSnippet()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
vnoremap <Plug>ZenCodingCodePretty :call zencoding#codePretty()
nnoremap <Plug>ZenCodingAnchorizeSummary :call zencoding#anchorizeURL(1)
nnoremap <Plug>ZenCodingAnchorizeURL :call zencoding#anchorizeURL(0)
nnoremap <Plug>ZenCodingRemoveTag :call zencoding#removeTag()
nnoremap <Plug>ZenCodingSplitJoinTagNormal :call zencoding#splitJoinTag()
vnoremap <Plug>ZenCodingMergeLines :call zencoding#mergeLines()
nnoremap <Plug>ZenCodingToggleComment :call zencoding#toggleComment()
nnoremap <Plug>ZenCodingImageSize :call zencoding#imageSize()
nnoremap <Plug>ZenCodingPrev :call zencoding#moveNextPrev(1)
nnoremap <Plug>ZenCodingNext :call zencoding#moveNextPrev(0)
vnoremap <Plug>ZenCodingBalanceTagOutwardVisual :call zencoding#balanceTag(-2)
nnoremap <Plug>ZenCodingBalanceTagOutwardNormal :call zencoding#balanceTag(-1)
vnoremap <Plug>ZenCodingBalanceTagInwardVisual :call zencoding#balanceTag(2)
nnoremap <Plug>ZenCodingBalanceTagInwardNormal :call zencoding#balanceTag(1)
nnoremap <Plug>ZenCodingExpandWord :call zencoding#expandAbbr(1,"")
nnoremap <Plug>ZenCodingExpandNormal :call zencoding#expandAbbr(3,"")
vnoremap <Plug>ZenCodingExpandVisual :call zencoding#expandAbbr(2,"")
nnoremap <silent> <SNR>47_yrrecord :call YRRecord3()
vmap <silent> <Plug>IMAP_JumpBack `<i=IMAP_Jumpfunc('b', 0)
vmap <silent> <Plug>IMAP_JumpForward i=IMAP_Jumpfunc('', 0)
vmap <silent> <Plug>IMAP_DeleteAndJumpBack "_<Del>i=IMAP_Jumpfunc('b', 0)
vmap <silent> <Plug>IMAP_DeleteAndJumpForward "_<Del>i=IMAP_Jumpfunc('', 0)
nmap <silent> <Plug>IMAP_JumpBack i=IMAP_Jumpfunc('b', 0)
nmap <silent> <Plug>IMAP_JumpForward i=IMAP_Jumpfunc('', 0)
xnoremap <silent> <Plug>NERDCommenterUncomment :call NERDComment("x", "Uncomment")
nnoremap <silent> <Plug>NERDCommenterUncomment :call NERDComment("n", "Uncomment")
xnoremap <silent> <Plug>NERDCommenterAlignBoth :call NERDComment("x", "AlignBoth")
nnoremap <silent> <Plug>NERDCommenterAlignBoth :call NERDComment("n", "AlignBoth")
xnoremap <silent> <Plug>NERDCommenterAlignLeft :call NERDComment("x", "AlignLeft")
nnoremap <silent> <Plug>NERDCommenterAlignLeft :call NERDComment("n", "AlignLeft")
nnoremap <silent> <Plug>NERDCommenterAppend :call NERDComment("n", "Append")
xnoremap <silent> <Plug>NERDCommenterYank :call NERDComment("x", "Yank")
nnoremap <silent> <Plug>NERDCommenterYank :call NERDComment("n", "Yank")
xnoremap <silent> <Plug>NERDCommenterSexy :call NERDComment("x", "Sexy")
nnoremap <silent> <Plug>NERDCommenterSexy :call NERDComment("n", "Sexy")
xnoremap <silent> <Plug>NERDCommenterInvert :call NERDComment("x", "Invert")
nnoremap <silent> <Plug>NERDCommenterInvert :call NERDComment("n", "Invert")
nnoremap <silent> <Plug>NERDCommenterToEOL :call NERDComment("n", "ToEOL")
xnoremap <silent> <Plug>NERDCommenterNested :call NERDComment("x", "Nested")
nnoremap <silent> <Plug>NERDCommenterNested :call NERDComment("n", "Nested")
xnoremap <silent> <Plug>NERDCommenterMinimal :call NERDComment("x", "Minimal")
nnoremap <silent> <Plug>NERDCommenterMinimal :call NERDComment("n", "Minimal")
xnoremap <silent> <Plug>NERDCommenterToggle :call NERDComment("x", "Toggle")
nnoremap <silent> <Plug>NERDCommenterToggle :call NERDComment("n", "Toggle")
xnoremap <silent> <Plug>NERDCommenterComment :call NERDComment("x", "Comment")
nnoremap <silent> <Plug>NERDCommenterComment :call NERDComment("n", "Comment")
nnoremap <silent> <F7> :TagbarToggle 
xmap <BS> "-d
cnoremap  <Home>
cnoremap  <End>
imap S <Plug>ISurround
imap s <Plug>Isurround
inoremap <silent> 	 =snipMate#TriggerSnippet()
imap <NL> <Plug>IMAP_JumpForward
cnoremap  
imap  <Plug>SuperTabForward
imap  <Plug>SuperTabBackward
inoremap <silent> 	 =snipMate#ShowAvailableSnips()
imap  <Plug>Isurround
inoremap 5 yypVr^A
inoremap 4 yypVr-A
inoremap 3 yypVr=A
inoremap 2 yyPVr*yyjpA
inoremap 1 yyPVr#yyjpA
imap A <Plug>ZenCodingAnchorizeSummary
imap a <Plug>ZenCodingAnchorizeURL
imap k <Plug>ZenCodingRemoveTag
imap j <Plug>ZenCodingSplitJoinTagInsert
imap / <Plug>ZenCodingToggleComment
imap i <Plug>ZenCodingImageSize
imap N <Plug>ZenCodingPrev
imap n <Plug>ZenCodingNext
imap D <Plug>ZenCodingBalanceTagOutwardInsert
imap d <Plug>ZenCodingBalanceTagInwardInsert
imap ; <Plug>ZenCodingExpandWord
imap , <Plug>ZenCodingExpandAbbr
cmap cd. lcd %:p:h
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set autoread
set background=dark
set backspace=2
set balloonexpr=CuteErrorBalloon()
set clipboard=unnamed
set completeopt=menu
set copyindent
set expandtab
set fileencodings=ucs-bom,utf-8,gbk,gb2312,latin1
set grepprg=grep\ -nH\ $*
set guifont=Osaka-Mono:h16
set guitablabel=%M%t
set helplang=en
set history=50
set hlsearch
set ignorecase
set incsearch
set isident=@,48-57,_,192-255,$
set langmenu=none
set laststatus=2
set matchpairs=(:),{:},[:],<:>
set mouse=a
set omnifunc=csscomplete#CompleteCSS
set printexpr=system('open\ -a\ Preview\ '.v:fname_in)\ +\ v:shell_error
set ruler
set runtimepath=~/.vim,~/.vim/bundle/CuteErrorMarker,~/.vim/bundle/OmniCppComplete,~/.vim/bundle/VisIncr,~/.vim/bundle/ack.vim,~/.vim/bundle/autoclose,~/.vim/bundle/command-t,~/.vim/bundle/easymotion,~/.vim/bundle/indent-motion,~/.vim/bundle/javacomplete,~/.vim/bundle/matchit,~/.vim/bundle/nerdcommenter,~/.vim/bundle/nerdtree,~/.vim/bundle/powerline,~/.vim/bundle/pythoncomplete,~/.vim/bundle/scala-vim-support,~/.vim/bundle/snipmate-snippets,~/.vim/bundle/supertab,~/.vim/bundle/surround,~/.vim/bundle/tagbar,~/.vim/bundle/tlib_vim,~/.vim/bundle/vim-addon-mw-utils,~/.vim/bundle/vim-coffee-script,~/.vim/bundle/vim-javascript,~/.vim/bundle/vim-latex,~/.vim/bundle/vim-snipmate,~/.vim/bundle/xmledit,~/.vim/bundle/yankring,~/.vim/bundle/zencoding-vim,~/Downloads/MacVim-snapshot-66/MacVim.app/Contents/Resources/vim/vimfiles,~/Downloads/MacVim-snapshot-66/MacVim.app/Contents/Resources/vim/runtime,~/Downloads/MacVim-snapshot-66/MacVim.app/Contents/Resources/vim/vimfiles/after,~/.vim/bundle/OmniCppComplete/after,~/.vim/bundle/vim-coffe
set shiftwidth=4
set showmatch
set smartcase
set smarttab
set softtabstop=4
set statusline=\ %{HasPaste()}%<%-15.25(%f%)%m%r%h\ %w\ \ \ \ \ [%{&ff}/%Y]\ \ \ %<%20.30(%{hostname()}:%{CurDir()}%)\ %=%-10.(%l,%c%V%)\ %p%%/%L
set termencoding=utf-8
set timeoutlen=500
set viminfo='10,\"100,:20,%,n~/.viminfo
set wildignore=*.o,*.class,*.pyc
set wildmenu
set window=44
set winminheight=0
set winminwidth=0
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/Projects/XCode/Cocos2d-X/__graduation_project/ResourceEditor
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +28 crop.py
badd +12 ~/Documents/æ¯•ä¸šè®¾è®¡/md2pdf.py
badd +499 ~/.bash_history
badd +498 proj.vim
badd +1 npcs.html
badd +16 Projects/Expresso/ResourceEditor/static/REApp.js
badd +6 Projects/Expresso/ResourceEditor/index.html
badd +3 Projects/Expresso/ResourceEditor/static/RENavigatorBar.js
badd +20 Projects/Expresso/ResourceEditor/npcs.html
badd +43 ResourceEditor.js
badd +1 static/REApp.js
badd +1 static/REUtils.js
badd +1 static/RENavigatorBar.js
badd +342 weapons.html
badd +43 static/REWeapon.js
badd +43 static/REOre.js
badd +118 static/REObjectIdEditor.js
badd +43 static/main.css
badd +43 static/REDiceEditor.js
badd +48 static/RECriticalHitRangeEditor.js
badd +425 armors.html
badd +15 static/REEffect.js
badd +1 ores.html
badd +1 maps.html
badd +1 static/RETMXMapUtils.js
badd +1 scenes.html
args static/REOre.js static/REWeapon.js
edit npcs.html
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
edit npcs.html
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
inoremap <buffer> \> >
inoremap <buffer> \. >
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s:<!--,m:\ \ \ \ ,e:-->
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'html'
setlocal filetype=html
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=HtmlIndentGet(v:lnum)
setlocal indentkeys=o,O,*<Return>,<>>,{,}
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'html'
setlocal syntax=html
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 397 - ((19 * winheight(0) + 21) / 43)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
397
normal! 0121l
tabedit static/REApp.js
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
edit static/REApp.js
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=//%s
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'javascript'
setlocal filetype=javascript
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=GetJavascriptIndent()
setlocal indentkeys=0{,0},0),0],!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=javascriptcomplete#CompleteJS
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'javascript'
setlocal syntax=javascript
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 9 - ((7 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
9
normal! 042l
tabedit static/REUtils.js
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
edit static/REUtils.js
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=//%s
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'javascript'
setlocal filetype=javascript
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=GetJavascriptIndent()
setlocal indentkeys=0{,0},0),0],!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=javascriptcomplete#CompleteJS
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'javascript'
setlocal syntax=javascript
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 31 - ((30 * winheight(0) + 21) / 43)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
31
normal! 010l
tabedit static/RENavigatorBar.js
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
edit static/RENavigatorBar.js
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=//%s
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'javascript'
setlocal filetype=javascript
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=GetJavascriptIndent()
setlocal indentkeys=0{,0},0),0],!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=javascriptcomplete#CompleteJS
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'javascript'
setlocal syntax=javascript
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 23 - ((19 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
23
normal! 063l
tabedit static/main.css
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
2argu
edit static/main.css
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s1:/*,mb:*,ex:*/
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'css'
setlocal filetype=css
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=^\\s*@import\\s\\+\\%(url(\\)\\=
setlocal includeexpr=
setlocal indentexpr=GetCSSIndent()
setlocal indentkeys=0{,0},!^F,o,O
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=csscomplete#CompleteCSS
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'css'
setlocal syntax=css
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 43 - ((37 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
43
normal! 08l
tabedit weapons.html
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
1argu
edit weapons.html
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
inoremap <buffer> \> >
inoremap <buffer> \. >
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s:<!--,m:\ \ \ \ ,e:-->
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'html'
setlocal filetype=html
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=HtmlIndentGet(v:lnum)
setlocal indentkeys=o,O,*<Return>,<>>,{,}
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'html'
setlocal syntax=html
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 342 - ((19 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
342
normal! 069l
tabedit ores.html
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
2argu
edit ores.html
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
inoremap <buffer> \> >
inoremap <buffer> \. >
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s:<!--,m:\ \ \ \ ,e:-->
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'html'
setlocal filetype=html
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=HtmlIndentGet(v:lnum)
setlocal indentkeys=o,O,*<Return>,<>>,{,}
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'html'
setlocal syntax=html
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 448 - ((19 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
448
normal! 025l
tabedit armors.html
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
edit armors.html
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
inoremap <buffer> \> >
inoremap <buffer> \. >
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s:<!--,m:\ \ \ \ ,e:-->
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'html'
setlocal filetype=html
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=HtmlIndentGet(v:lnum)
setlocal indentkeys=o,O,*<Return>,<>>,{,}
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'html'
setlocal syntax=html
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 526 - ((29 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
526
normal! 025l
tabedit static/RETMXMapUtils.js
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
edit static/RETMXMapUtils.js
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=//%s
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'javascript'
setlocal filetype=javascript
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=GetJavascriptIndent()
setlocal indentkeys=0{,0},0),0],!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=javascriptcomplete#CompleteJS
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},0)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'javascript'
setlocal syntax=javascript
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 37 - ((32 * winheight(0) + 19) / 38)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
37
normal! 011l
tabedit scenes.html
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
edit scenes.html
vnoremap <buffer> <silent> ,a" `>a"`<i"
vnoremap <buffer> <silent> ,a> `>a>`<i<
vnoremap <buffer> <silent> ,a< `>a>`<i<
vnoremap <buffer> <silent> ,a] `>a]`<i[
vnoremap <buffer> <silent> ,a[ `>a]`<i[
vnoremap <buffer> <silent> ,a) `>a)`<i(
vnoremap <buffer> <silent> ,a( `>a)`<i(
vnoremap <buffer> <silent> ,a' `>a'`<i'
vnoremap <buffer> <silent> ,a} `>a}`<i{
vnoremap <buffer> <silent> ,a{ `>a}`<i{
vnoremap <buffer> <silent> ,a« `>a«`<iÂ
vnoremap <buffer> <silent> ,aÂ `>a«`<iÂ
vnoremap <buffer> <silent> ,a` `>a``<i`
inoremap <buffer> \> >
inoremap <buffer> \. >
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal balloonexpr=
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=s:<!--,m:\ \ \ \ ,e:-->
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'html'
setlocal filetype=html
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=2
setlocal imsearch=2
setlocal include=
setlocal includeexpr=
setlocal indentexpr=HtmlIndentGet(v:lnum)
setlocal indentkeys=o,O,*<Return>,<>>,{,}
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,$
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal nomacmeta
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=%!Pl#Statusline({'r':\ '%(%(%#Ple7ffffffa0d70000b#\ %{Powerline#Functions#GetMode()}\ %)%#Pla0d70000f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'n':\ '%(%(%#Pl16005f0094afd700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pl94afd700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'v':\ '%(%(%#Pl58870000d0ff8700b#\ %{Powerline#Functions#GetMode()}\ %)%#Pld0ff8700f0585858b#%)%(\ %(%#Plc4ff0000f0585858b#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7fffffff0585858b#%t\ %)%(%#Plc4ff0000f0585858b#%M\ %)%(%#Plc4ff0000f0585858b#%H%W\ %)%#Plf0585858ec303030b#%)%<%#Pl00ff0000ec303030N#%=%(%#Plec303030ec303030N#%(%#Plf79e9e9eec303030N#\ %{&fileformat}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Plf79e9e9eec303030N#â”‚%(%#Plf79e9e9eec303030N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Plf0585858ec303030N#%(%#Plfabcbcbcf0585858N#\ %3p%%\ %)%)%(%#Plfcd0d0d0f0585858b#%(%#Plec303030fcd0d0d0b#\ LN\ %3l%)%(%#Plf4808080fcd0d0d0N#\ C\ %-2c%)\ %)',\ 'i':\ '%(%(%#Pl17005f5fe7ffffffb#\ %{Powerline#Functions#GetMode()}\ %)%#Ple7ffffff1f0087afb#%)%(\ %(%#Plc4ff00001f0087afb#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Ple7ffffff1f0087afb#%t\ %)%(%#Plc4ff00001f0087afb#%M\ %)%(%#Plc4ff00001f0087afb#%H%W\ %)%#Pl1f0087af18005f87b#%)%<%#Pl00ff000018005f87N#%=%(%#Pl18005f8718005f87N#%(%#Pl7587d7ff18005f87N#\ %{&fileformat}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{(&fenc\ ==\ \"\"\ ?\ &enc\ :\ &fenc)}\ %)%)%(%#Pl7587d7ff18005f87N#â”‚%(%#Pl7587d7ff18005f87N#\ %{strlen(&ft)\ ?\ &ft\ :\ \"no\ ft\"}\ %)%)%(%#Pl1f0087af18005f87N#%(%#Pl7587d7ff1f0087afN#\ %3p%%\ %)%)%(%#Pl7587d7ff1f0087afb#%(%#Pl17005f5f7587d7ffb#\ LN\ %3l%)%(%#Pl17005f5f7587d7ffN#\ C\ %-2c%)\ %)',\ 'N':\ '%(\ %(%#Pl58870000eb262626N#%{&readonly\ ?\ \"RO\"\ :\ \"\"}\ %)%(%#Plf58a8a8aeb262626b#%t\ %)%(%#Pl58870000eb262626N#%M\ %)%(%#Pl58870000eb262626N#%H%W\ %)%#Pleb262626e9121212b#%)%<%#Pl00ff0000e9121212N#%=%(%#Pleb262626e9121212N#%(%#Plef4e4e4eeb262626N#\ %3p%%\ %)%)%(%#Plf58a8a8aeb262626b#â”‚%(%#Plf58a8a8aeb262626b#\ LN\ %3l%)%(%#Plf1626262eb262626N#\ C\ %-2c%)\ %)'},1)
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'html'
setlocal syntax=html
endif
setlocal tabstop=8
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 522 - ((9 * winheight(0) + 21) / 43)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
522
normal! 030l
tabnext 10
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
