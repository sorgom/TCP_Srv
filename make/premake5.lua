--  ============================================================
--  premake5 build rules
--  TCP_Srv_Echo based on TCP_Srv_Base
--  configurations: 
--  - silent:
--      - no debug
--      - no output
--      - optimized for speed
--  - verbose:
--      - verbose output
--      - debug infos
--      - no optimization
--  - vsmall:
--      - same as verbose
--      - but with small buffer sizes (16 bytes)
--      - and long select timeout (1 second)
--  ============================================================
--  created by Manfred Sorgo

workspace 'TCP_Srv_Echo'
    configurations { 'silent', 'verbose', 'vsmall' }
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

        filter { 'configurations:vsmall' }
            defines { 
                'DEBUG', 'VERBOSE',
                'SELECT_MILLI_SECONDS=1000',
                'READ_BUFFER_SIZE=16'
             }
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
