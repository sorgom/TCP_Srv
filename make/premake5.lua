--  ============================================================
--  premake5 build rules
--  configurations: 
--  A) TCP_Srv_Echo based on TCP_Srv_Base
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
--  B) modified ChatGPT solution
--  - gpt:
--      - no debug
--      - no output
--      - optimized for speed
--  ============================================================
--  created by Manfred Sorgo

filesTCP = { '../code/TCP_Srv_*.cpp', '../code/Trace.cpp' }
filesGPT = { '../code/GPT_*.cpp' }

workspace 'EchoSrv'
    configurations { 'prod', 'verbose', 'vsmall', 'gpt' }
    language 'C++'
    targetsuffix  '_%{cfg.name}'

    project 'EchoSrv'
        kind 'ConsoleApp'

        filter { 'configurations:prod' }
            files { filesTCP }
            defines { 'NDEBUG' }
            symbols 'Off'
            optimize 'Speed'
    
        filter { 'configurations:verbose' }
            files { filesTCP }
            defines { 'DEBUG', 'VERBOSE' }
            symbols 'On'
            optimize 'Off'

        filter { 'configurations:vsmall' }
            files { filesTCP }
            defines { 
                'DEBUG', 'VERBOSE',
                'SELECT_MILLI_SECONDS=1000',
                'READ_BUFFER_SIZE=8'
             }
            symbols 'On'
            optimize 'Off'

        filter { 'configurations:gpt' }
            files { filesGPT }
            defines { 'NDEBUG' }
            symbols 'Off'
            optimize 'Speed'

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
