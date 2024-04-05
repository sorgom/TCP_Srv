workspace 'EchoSrv_ChatGPT'
    filter { 'action:gmake*' }
        configurations { 'ChatGPT' }
        language 'C++'

        project 'EchoSrv'
            kind 'ConsoleApp'
            files { '*.cpp' }
            buildoptions { '-std=c++17 -pedantic-errors -Werror -Wall' }
            objdir 'obj/gcc/'
            linkoptions { '-pthread' }
            targetdir 'bin'
