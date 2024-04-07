workspace 'EchoSrvGPT'
    configurations { 'ChatGPT' }
    language 'C++'

    project 'EchoSrvGPT'
        kind 'ConsoleApp'
        files { '*.cpp' }
        filter { 'action:gmake*' }
            buildoptions { '-std=c++17 -pedantic-errors -Werror -Wall' }
            objdir 'obj/gcc/'
            linkoptions { '-pthread' }
            targetdir 'bin'
        
        filter { 'action:vs*' }
            warnings 'high'
            buildoptions { '/std:c++17 /W4' }
            objdir 'obj/vs/'
            targetdir 'exe'
