--  ============================================================
--  premake5 build rules
--  ============================================================
includesBase = { '../include' }

buildOptsGcc = { '-std=c++17 -pedantic-errors -Werror -Wall' }
buildOptsVS = { '/std:c++17 /W4' }

dummy = { '../sample/dummy.cpp' }

--  ============================================================
--  TCP_Srv_Base library
--  ============================================================
workspace 'TCP_Srv_Base'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj'
    includedirs { includesBase }
    targetdir 'lib'

    filter { 'action:vs*' }
        warnings 'high'
        buildoptions { buildOptsVS }

    filter { 'action:gmake*' }
        buildoptions { buildOptsGcc }

    project 'TCP_Srv_Base'
        kind 'StaticLib'
        targetdir 'lib'
        files { '../src/*.cpp' }

--  ============================================================
--  sample of method threads
--  ============================================================
workspace 'sample_Method_Threads'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj'

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
    objdir 'obj'

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
    objdir 'obj'

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
