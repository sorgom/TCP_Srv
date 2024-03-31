--  ============================================================
--  premake5 build rules for gcc (gmake2):
--  ============================================================
includesBase = { '../include' }

buildOptsGcc = { '-std=c++17 -pedantic-errors -Werror -Wall' }
buildOptsVS = { '/std:c++17 /W4' }

dummy = { '../sample/dummy.cpp' }

--  ============================================================
--  > TCP_Srv_Base library
--  ->  VS: lib/TCP_Srv_Base.lib
--  ->  gcc: lib/libTCP_Srv_Base.a
--  ============================================================
workspace '01_TCP_Srv_Base'
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
--  > sample: method threads
--  ->  gcc: bin/sample_Method_Threads
--  ->  VS: exe/sample_Method_Threads.exe
--  ============================================================
workspace '11_sample_Method_Threads'
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
--  > Microsoft sample: echo server
--  VS only
--  ->  exe/sample_Srv_MicroSoft_VS.exe
--  ============================================================
workspace '12_sample_Srv_MicroSoft_VS'
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
--  > Tenouk sample: echo server
--  ->  gcc: bin/sample_Srv_Tenouk
--  ->  VS: exe/sample_Srv_Tenouk.exe
--  ============================================================
workspace '13_sample_Srv_Tenouk'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj'

    project 'Tenouk_SampleSrv'
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
