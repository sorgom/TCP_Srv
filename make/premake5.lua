--  ============================================================
--  premake5 build rules
--  ============================================================
includesBase = { '../include' }

buildOptsGcc = '-std=c++17 -pedantic-errors -Werror -Wall '
buildOptsVS = '/std:c++17 /W4 ' 

dummy = { '../sample/dummy.cpp' }

--  ============================================================
--  sample TCP_Srv_Echo based on TCP_Srv_Base
--  with tracing off
--  ============================================================
workspace 'TCP_Srv_Echo_silent'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'

    project 'TCP_Srv_Echo_silent'
        includedirs { includesBase }
        kind 'ConsoleApp'
        files { '../src/*.cpp', '../sample/TCP_Srv_Echo*.cpp' }

        filter { 'action:vs*' }
            warnings 'high'
            buildoptions { buildOptsVS .. '/wd4100' }
            targetdir 'exe'
    
        filter { 'action:gmake*' }
            buildoptions { buildOptsGcc }
            linkoptions { '-pthread' }
            targetdir 'bin'

--  ============================================================
--  sample TCP_Srv_Echo based on TCP_Srv_Base
--  with tracing on
--  ============================================================
workspace 'TCP_Srv_Echo_verbose'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'

    project 'TCP_Srv_Echo_verbose'
        includedirs { includesBase }
        kind 'ConsoleApp'
        files { '../src/*.cpp', '../sample/TCP_Srv_Echo*.cpp' }

        filter { 'action:vs*' }
            warnings 'high'
            buildoptions { buildOptsVS .. '/DTRACE_ON' }
            targetdir 'exe'
    
        filter { 'action:gmake*' }
            buildoptions { buildOptsGcc .. '-DTRACE_ON' }
            linkoptions { '-pthread' }
            targetdir 'bin'

--  ============================================================
--  sample of method threads
--  ============================================================
workspace 'sample_Method_Threads'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'

    filter { 'action:vs*' }
        warnings 'high'
        buildoptions { buildOptsVS }
        targetdir 'exe'

    filter { 'action:gmake*' }
        buildoptions { buildOptsGcc }
        linkoptions { '-pthread' }
        targetdir 'bin'

    project 'sample_Method_Threads'
        kind 'ConsoleApp'
        files { '../sample/sample_Method_Threads.cpp' }

--  ============================================================
--  sample echo server Microsoft VS only
--  ============================================================
workspace 'sample_Srv_MicroSoft_VS'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'

    project 'sample_Srv_MicroSoft_VS'
        kind 'ConsoleApp'

        filter { 'action:vs*' }
            warnings 'high'
            buildoptions { buildOptsVS }
            targetdir 'exe'
            files { '../sample/sample_Srv_MicroSoft_VS.cpp' }

        filter { 'action:gmake*' }
            buildoptions { buildOptsGcc }
            files { dummy }
            targetdir 'bin'

--  ============================================================
--  sample echo server Tenouk
--  ============================================================
workspace 'sample_Srv_Tenouk'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'

    project 'sample_Srv_Tenouk'
        kind 'ConsoleApp'
        includedirs { includesBase }
        files { '../sample/sample_Srv_Tenouk.cpp' }

        filter { 'action:vs*' }
            warnings 'high'
            buildoptions { buildOptsVS }
            targetdir 'exe'

        filter { 'action:gmake*' }
            buildoptions { buildOptsGcc }
            targetdir 'bin'
