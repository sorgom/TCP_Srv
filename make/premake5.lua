--  ============================================================
--  premake5 build rules for gcc (gmake2):
--  ============================================================

buildOptsGcc = { '-std=c++17 -pedantic-errors -Werror -Wall' }
includesBase = { '../include' }
--  ============================================================
--  VS setup
--  warning level: 4 (high)
--  
--  suppressed warnings application:
--  -   4100 unreferenced formal parameter
--  -   4103 change of alignement with header inclusion
--      this is intended behaviour of packBegin.h / packEnd.h
--  additional suppressed warnings test code:
--  -   4127 suggested 'if constexpr' 
--      warning caused by CppUTest headers code
--  ============================================================
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
    objdir 'obj/%{prj.name}'
    includedirs { includesBase }
    targetdir 'lib'

    filter { 'action:vs*' }
        buildoptions { buildOptsVS }

    filter { 'action:gmake*' }
        buildoptions { buildOptsGcc }

    project 'TCP_Srv_Base'
        kind 'StaticLib'
        targetdir 'lib'
        files { '../src/*.cpp' }

--  ============================================================
--  > sample: metod threads
--  ->  gcc: bin/11_sampleMethodTreads
--  ->  VS: exe/11_sampleMethodTreads.exe
--  ============================================================
workspace '11_SampleMethodTreads'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'

    filter { 'action:vs*' }
        buildoptions { buildOptsVS }
        targetdir 'exe'

    filter { 'action:gmake*' }
        buildoptions { buildOptsGcc }
        linkoptions { '-pthread' }
        targetdir 'bin'

    project '11_sampleMethodTreads'
        kind 'ConsoleApp'
        files { '../sample/MethodThreads.cpp' }

--  ============================================================
--  > Microsoft sample: echo server
--  VS only
--  ->  exe/12_MS_SamplSrv.exe
--  ============================================================
workspace '12_MS_SamplSrv'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'
    kind 'ConsoleApp'

    project '12_MS_SamplSrv'

        filter { 'action:vs*' }
            buildoptions { buildOptsVS }
            targetdir 'exe'
            files { '../sample/MS_SampleSrv.cpp' }

        filter { 'action:gmake*' }
            buildoptions { buildOptsGcc }
            files { dummy }
            targetdir 'bin'

--  ============================================================
--  > Tenouk sample: echo server
--  VS only
--  ->  exe/12_Tenouk_SampleSrv.exe
--  ============================================================
workspace '13_Tenouk_SampleSrv'
    configurations { 'ci' }
    language 'C++'
    objdir 'obj/%{prj.name}'
    kind 'ConsoleApp'
    includedirs { includesBase }

    project '13_MS_SamplSrv'

        filter { 'action:vs*' }
            buildoptions { buildOptsVS }
            targetdir 'exe'
            files { '../sample/Tenouk_SampleSrv.cpp' }

        filter { 'action:gmake*' }
            buildoptions { buildOptsGcc }
            files { dummy }
            targetdir 'bin'
                        