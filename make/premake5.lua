--  ============================================================
--  premake5 build rules
--  ============================================================

buildOptsGcc = '-std=c++17 -pedantic-errors -Werror -Wall '
buildOptsVS = '/std:c++17 /W4 ' 

--  ============================================================
--  sample TCP_Srv_Echo based on TCP_Srv_Base
--  with tracing off
--  ============================================================
workspace 'TCP_Srv_Echo_silent'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'

    project 'TCP_Srv_Echo_silent'
        kind 'ConsoleApp'
        files { '../code/*.cpp' }

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
        kind 'ConsoleApp'
        files { '../code/*.cpp' }

        filter { 'action:vs*' }
            warnings 'high'
            buildoptions { buildOptsVS .. '/DTRACE_ON' }
            targetdir 'exe'
    
        filter { 'action:gmake*' }
            buildoptions { buildOptsGcc .. '-DTRACE_ON' }
            linkoptions { '-pthread' }
            targetdir 'bin'
