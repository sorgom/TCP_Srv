--  ============================================================
--  premake5 build rules
--  TCP_Srv_Echo based on TCP_Srv_Base
--  configurations: 
--  - prod:
--      - no debug
--      - no output
--      - optimized for speed
--  - verbose:
--      - verbose output
--      - debug infos
--      - no optimization
--  - vsmall:
--      - same as verbose
--      - but with small buffer sizes (8 bytes)
--      - and long select timeout (1000 ms)
--  ============================================================
--  created by Manfred Sorgo

workspace 'EchoSrv'
    configurations { 'prod', 'verbose', 'vsmall' }
    language 'C++'
    targetsuffix  '_%{cfg.name}'

    project 'EchoSrv'
        kind 'ConsoleApp'
        files { '../code/*.cpp' }

        filter { 'configurations:prod' }
            defines { 'NDEBUG' }
            symbols 'Off'
            optimize 'Speed'
    
        filter { 'configurations:verbose' }
            defines { 'DEBUG', 'VERBOSE' }
            symbols 'On'
            optimize 'Off'

        filter { 'configurations:vsmall' }
            defines { 
                'DEBUG', 'VERBOSE',
                'SELECT_MILLI_SECONDS=1000',
                'READ_BUFFER_SIZE=8'
             }
            symbols 'On'
            optimize 'Off'

        filter { 'action:vs*' }
            warnings 'high'
            buildoptions { '/std:c++17 /W4' }
            objdir 'obj/vs/'
            targetdir 'exe'

        filter { 'action:gmake*' }
            buildoptions { '-std=c++17 -pedantic-errors -Werror -Wall' }
            objdir 'obj/gcc/'
            linkoptions { '-pthread' }
            targetdir 'bin'
