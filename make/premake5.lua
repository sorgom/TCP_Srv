--  ============================================================
--  premake5 build rules
--  TCP_Srv_Echo based on TCP_Srv_Base
--  configurations: silent, verbose, deb
--  ============================================================
--  created by Manfred Sorgo

workspace 'TCP_Srv_Echo'
    configurations { 'silent', 'verbose' }
    language 'C++'
    optimize 'On'

    project 'TCP_Srv_Echo'
        kind 'ConsoleApp'
        files { '../code/*.cpp' }

        filter { 'configurations:silent' }
            defines { 'NDEBUG' }
            symbols 'Off'
            optimize 'Speed'
    
        filter { 'configurations:verbose' }
            defines { 'DEBUG', 'VERBOSE' }
            symbols 'On'
            optimize 'Off'

        filter { 'action:vs*' }
            buildoptions { '/std:c++17 /W4 /wd4100' }
            objdir 'obj/vs/'
            warnings 'high'
            targetdir 'exe'

        filter { 'action:gmake*' }
            buildoptions { '-std=c++17 -pedantic-errors -Werror -Wall' }
            objdir 'obj/gcc/'
            linkoptions { '-pthread' }
            targetdir 'bin'
