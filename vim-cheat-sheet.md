# Vim cheat sheet

Basic commands. Requires to know the construction rules.

Fundamentals:

    $ vimtutor

## Help
* `:help` - Main help file.
    * `Ctrl+]` - Open help for cursor topic.
    * `Ctrl+i` - Go forward.
    * `Ctrl+o` - Go backward.
* `:h <item>` - Item help.
* `:h '<option>'` - Option help.
* `:h <prefix> Tab` - Loop through help items starting with prefix.
* `:h <prefix> Ctrl+d` - Print available help items starting with prefix.
* `:h option-list` - Short explanation for all options.
* `:h option-summary` - Options summary.

Examples:
* `:h number` - Help for the number command.
* `:h 'number'` - Help for the number option.
* `:h ctrl-g` - Help for the control-g command.
* `:h :f` - Help for the :f command.

## Save and quit
* `:w` - Write changes in buffer to file.
* `:wq` - Save and quit.
* `:q` - Quit.
* `:q!` - Force quit without save.

## Navigation
* `h`, `j`, `k`, `l` - Move cursor left, down, up, right.
* `w` - Move cursor one word forward.
* `b` - Move cursor one word backward.
* `0` - Move cursor to line start.
* `$` - Move cursor to line end.
* `^` - Move cursor to first non-blank character in the line.
* `gg` - Move to the first line.
* `G` - Move to the last line.
* `Ctrl+f` - Scroll down a page.
* `Ctrl+b` - Scroll up a page.
* `Ctrl-d` - Scroll down half a screen.
* `Ctrl-u` - Scroll up half a screen.
* `z return`, `z.`, `z-` - Redraw the window with the cursor line on top, center, bottom.

## Basic Editing
* `i` - Switch to insert mode to modify the content of the file buffer.
* `a` - Move the cursor one position before switching to insert mode.
* `I` - Switch to inset mode at line beginning.
* `A` - Switch to inset mode at line ending.
* `o` - Create a new line below and switch to insert mode.
* `O` - Create a new line above and switch to insert mode.
* `x` - Delete current character.
* `X` - Delete left character.
* `d0` - Delete cursor line from current position to the beginning.
* `d$` or `D` - Delete cursor line from current position to end of line.
* `dd` - Delete cursor line.
* `dj` - Delete next line.
* `dk` - Delete previous line.
* `r` - Replace single character.
* `R` - Replace multiple characters.
* `cc` or `C` - Overwrite cursor line.
* `J` - Join current and the next line inserting a space between.
* `gJ` - Join current and the next line not inserting a space between.
* `~` - Change letter case of character.
* `gUU` - Change line to upper-case.
* `guu` - Change line to lower-case.

## Yank & Put

* `yy` or `Y` - Yank the cursor line.
* `y /<pattern>` - Yank text between cursor position and search pattern.
* `p` - Put the last yanked / deleted text after the cursor position.
* `P` - Put the last yanked / deleted text in front of the cursor position.

## Text objects

* `yis` - Yank sentence.
* `yi"` - Yank text in quotes.
* `yi]` or `yi[` - Yank text in square brackets.
* `yi)` or `yib` - Yank text in round brackets (blocks).
* `daw` - Delete word.
* `dap` - Delete paragraph.
* `ciw` - Replace word.
* `cit` - Replace content in XML tags.

`a` includes spaces and newlines, `i` does not.

## Registers

* Yanked text is stored in `"0`.
* Deleted text is stored in `"1` to `"9`. The content is moved down and dropped after `"9`.
* In normal mode type the register and then the operation. Examples:
    * `"xyy` - Yank the current line in `"x`.
    * `"Xyy` - Append the current line to `"x`.
    * `"xp` - Insert the content of `"x` in the next line.

## Macros

* `q<reg>` - Start recording macro in register.
* `q` - Stop recording.
* `@<reg>` - Run selected macro.
* `<N>@<reg>` - Run macro N times.
* `@@` - Repeat the macro.

Example macro:

    :reg m
    "m   0i/* ^[A */^[j

Read '^[' as Esc. So `@m` transforms the current line into a C comment.

The macro can be saved in ~/.vimrc:

    let @m = '0i/* ^[A */^[j'

The Esc symbol '^[' is entered with `Ctrl-v Esc` in insert mode.

## Visual Mode

* `v` - Character selection.
* `V` - Line selection.
* `Ctrl+v` - Block selection.
    * Allows to repeat an operation vertically at the same cursor position.
* `gv` - Highlights the previous selection.

Use the cursor keys or specify a text object for selection. Example:
* `vap` - The current paragraph is highlighted.

Then type in an operation. Example:
* `:center80` - The selected text is centered within a line width of 80 characters.

Moving the cursor within the selection:
* `o` - Jump to the beginning of the selection.
* `O` - Jump to the end of the selection.

## Search and replace
* `f<char>`, `F<char>`  - Search the next, previous occurrence of character in cursor line.
    * `;`, `,` - Move to the next, previous occurrence of the same character.
* `t<char>`, `T<char>`  - Like `f`, position the cursor in front / behind the character.
* `*`, `#` - Jump to next, previous occurrence of the cursor word.
* `/pattern`, `?pattern` - Search "pattern" in file forwards, backwards.
    * `n`, `N` - Find the next, previous position of "pattern".
* `:s/search-text/replace-text` - Substitute the first match of "search-text" with "replace-text".
* `:s/search-text/replace-text/g` - Substitute all matches in the cursor line.
* `:%s/search-text/replace-text/g` - Substitute all matches in the file.
* `:<from>,<to>s/<search>/<replace>/g` - Subst. text inside the line number range [from, to].
* `:/<start>/,/<end>/s/<search>/<replace>/g` - Subst. text between "start" and "end" pattern.

When '#' is used instead of '/' as the delimiter, a forward-slash in the search pattern doesn't need to be escaped.

## Indentation and alignment

* `>>`, `<<` - Shift line right/left.
* `:ce` - Center line.
* `:ri` - Right align line.
* `:le` - Left align line.

## Undo & Redo

* `u` - Undo operation.
* `Ctrl+r` - Redo operation.

## Buffers

A buffer represents a file in memory.

* `:ls` - List buffers.
    * FLags:
        * '%' - current buffer
        * 'a' - active buffer
        * 'h' - hidden buffer
        * '+' - modified buffer
        * '#' - alternate buffer
* `:bn`, `:bp` - Switch to the next / previous buffer. 
* `:bf`, `:bl` - Switch to the first / last buffer.
* `:b#` - Switch to the alternate buffer.
* `:b<number>` - Switch to the buffer with the given list number.
* `:wall` - Save changes for all buffers at once.
* `:qall` - Quit Vim when multiple buffers are loaded.
* `:badd <filename>` - Add buffer.
* `:bd` - Remove buffer.
* `:bufdo <command>` - Run command for all buffers.
    * Example `:bufdo %s/2019/2020/g`
* `:ba` - Display all buffers in a window.
* `:set hidden` - Suppress the warning when leaving a modified buffer.

With the _autowrite_ option, modifications are saved before leaving the buffer so no warning is displayed even if _nohidden_ is set.

## Windows

A window displays a buffer.

* `:sp [<filename>]` - Split window horizontally.
* `:vs [<filename>]` - Split window vertically.
* `Ctrl-w q` - Close window.
* `Ctrl-w w` - Set focus to next window.
* `Ctrl-w r` - Rotate windows.
* `Ctrl-w h`, `Ctrl-w j`, `Ctrl-w k`, `Ctrl-w l` - Navigate through windows.
* `Ctrl-w +`, `Ctrl-w -`, `Ctrl-w <`, `Ctrl-w >` - Change window size in the specified direction.
* `Ctrl-w _` - Maximize window height.
* `Ctrl-w |` - Maximize window width.
* `Ctrl-w =` - Make all windows the same size.
* `:windo <command>` - Apply a command to all windows.

## Vim Options
* `:set <option>` - Activate option.
* `:set no<option>` - Deactivate option.
* `:set <option>!` - Toggle option.
* `:set <option>?` - Show the option's value.
* `:set` - Show the non-default options that are in use.
* `:options` - Allows to browse all options and activate any option by hitting Enter on the line that starts with "set".

Examples for default settings as seen in '/usr/share/vim/vim80/defaults.vim'
* _'background=dark' - The background color brightness._
* _'showcmd' - Show the command while typing it._
* _'wildmenu' - Command-line completion shows a list of matches._
* _'history=200' - The number of commands kept in the history._
* _'ruler' - The cursor coordinates are displayed at the bottom of the window._

Examples for recommended options as seen in '/usr/share/vim/vimrc'
* _'showmatch' - When insert a bracket quickly set the focus on the matching bracket if any._
* _'ignorecase' - Ignore letter case in search pattern._
* _'smartcase' - If an upper case letter is typed, 'ignorecase' has no effect._
* _'incsearch' - Highlight matches while typing the search pattern._
* _'autowrite' - On some commands automatically write changes to the file._
* _'hidden' - Doesn't warn when leaving a modified buffer._
* _'mouse=a' - Use Mouse to select text in visual mode. Middle mouse button inserts text._

Misc options
* _'number' - Show line numbers._
* _'hlsearch' - Highlight all search matches._
* _'linebreak' - Wraps long lines. Only visual - no linebreaks inserted. Check the 'breakat' option._
* _'autoindent' - Copy indent from current line when starting a new line._
* _'cindent' - Automatic C program indenting._
* _'expandtab' - Use spaces for Tabs._
* _'tabstop=8' - Number of spaces that a Tab in the file counts for._
* _'shiftwidth=4' - Number of spaces to use for each step of (auto)indent._
* _'smarttab' - Use 'shiftwidth' when inserting Tab._
* _'modeline' - Recognize modelines._
* _'textwidth=120' - Replace the last space with a linebreak to not exceed the textwidth._
    * _To get this working one option is to use this modeline:\
    /* vim: set formatoptions+=t : */_
* _'backup' - before changes are written to a file 'name.ext', vim creates a backup of the file in 'name.ext~'._
* _'cryptmethod=blowfish2' - Set the encryption method to "blowfish2"._
* _'list' - Show control symbols for white-space charcaters._
* _'cursorcolumn' - Highlight the screen column of the cursor._
* _'cursorline' - Highlight the screen line of the cursor._

**Some options may not work or cause unexpected behavior if not used in combination with other options.**

## .vimrc file

When started Vim checks the file $HOME/.vimrc for user preferred options.

~/.vimrc:

    set option[=value]

* `:version` - Display a desciption of the Vim binary. Besides the feature list and the compile options it also shows the path of the .vimrc files that Vim is checking.
* `:scriptnames` - Display the full pathname of all scripts and .vimrc files that were actually sourced when Vim was started.
* `:mkvimrc` - Create a default .vimrc file in the current directory.

## File type detection

The file type is recognized by the file extension or the content of the file. According to the file type different settings are selected for example the syntax highlighting.

* `:set ft?` - Show the detected file type.
* `:set ft=<type>` - Manually set the file type.
* `:filetype detect` - Run the file type detection.

The file type can be set in the modeline. For example the following modeline forces the file to be recognized as a C language file:

    /* vim: set ft=c : */

## Encryption

Vim can encrypt the file when it is saved. This happens automatically when a password was set for the file.

1. Set the encryption method:

    `:set cryptmethod=blowfish2`
1. Set the password:

    `:X`
    ```
    *****
    ```
1. Save & exit

    **Caution: if the user forgets the password the content is lost forever.**

## GUI version

Only if Vim has been compiled with GUI support.


    $ vim -g

or

    $ gvim

or

    $ vim
    :gui

### Copy & paste text between gvim and other applications

Check the following options:

    :set clipboard? mouse?
    clipboard=unnamed
    mouse=a

Mouse
* vim -> application
    * select text with mouse -> middle mouse button
* application -> vim
    * works the same

Keyboard
* vim -> application
    * copy selected text with `"+y` -> Ctrl-v
* application -> vim
    * Ctrl-c -> `"+p`

gvim settings: ~/.gvimrc.

## Spell checking

Activate spell checking for US english: `:set spell spelllang=en_us`

If no spell file is found it will be downloaded

    ```
    :set spell spelllang=de_20
    Cannot find spell file for "de" in utf-8
    Do you want me to try downloading it?
    Downloading de.utf-8.spl...
    ```

* jump forward, backward to wrong spelled word: `]s`, `[s`
* show list with word suggestions: `z=`

## Folding

Fold mutliple lines of text to a single line. The foldet text can be yanked, put and deleted like if it was a single line.

* `zf<selection>` - Create fold.
    * Example: `zfap` - Fold paragraph.
* `zo` - Open fold.
* `zc` - Close fold.

## Miscellaneous

* `:e <file>` - Open file for editing.
* `:browse e <directory>` - Shows a file browser for the specified directory. Selected files are opened for editing.
* `:shell`- Opens a new shell and switches to it. Return to Vim by exiting the shell.
* `:!<command>` - Execute shell command.
* `K` - Opens the man page for the keyword under the cursor.
* `ga` - Print the character value at the cursor position.
* `Ctrl-a`, `Ctrl-x` - When on a number increment / decrement it.
* `.`  - Repeat last change.
* `Ctrl+v <key>` - In insert mode enter a non-printable character.
    * Example: `Ctrl+v Esc` will enter the character 0x1B.
* `:echo <expr>` - Echoes expression.
    * Example: `:echo 2 * 2`
* `Ctrl+g` or `:f` - Print filename.
* `g Ctrl+g` - Print cursor position.

**EOF**
