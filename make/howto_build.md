# how to build
## gcc build
### preconditions
- make
- gcc
### make
- call make help to see available configurations
- call make clean with a configuration
- call make -j with a configuration

````shell
TCP_Srv/make$> make help
Usage: make [config=name] [target]

CONFIGURATIONS:
  prod
  verbose
  vsmall

TARGETS:
   all (default)
   clean

TCP_Srv/make$> make config=vsmall clean
Cleaning EchoSrv

TCP_Srv/make$> make -j config=vsmall
==== Building TCP_Srv_Echo (vsmall) ====
Creating obj/gcc/vsmall
Creating bin
TCP_Srv_Base.cpp
TCP_Srv_Echo.cpp
TCP_Srv_Echo_main.cpp
Linking EchoSrv
````
### use build.sh
call help
````shell
TCP_Srv/make$> ./build.sh -h
Usage: build.sh options [port] [locale]
options:
-c  clean ignored artifacts before
-r  <config> run binary with [port] [locale]
    with <config> in prod, verbose, vsmall
-p  premake5 makefiles
-h  this help
````
build (and run) e.g.:
- configuration _verbose_
- port 8090
- locale de_DE.UTF-8
````shell
TCP_Srv/make$> ./build.sh -cr vsmall 8090 de_DE.UTF-8
building congigurations ...
prod      [  5]
vsmall    [  6]
verbose   [  6]

starting bin/EchoSrv_vsmall 8090 de_DE.UTF-8

locale : de_DE.UTF-8
...
timeout:  1000 ms
buffer :     8 bytes
port   :  8090
````

## Visual Studio build
### preconditions
- premake5
- Visual Studio

### 1) generate VS solution using premake5
#### 1.1) find out suitable VS version using premake5 --help

````shell
$> premake5 --help
Usage: premake5 [options] action [arguments]

ACTIONS
 vs2005            Generate Visual Studio 2005 project files
 vs2008            Generate Visual Studio 2008 project files
 vs2010            Generate Visual Studio 2010 project files
 vs2012            Generate Visual Studio 2012 project files
 vs2013            Generate Visual Studio 2013 project files
 vs2015            Generate Visual Studio 2015 project files
 vs2017            Generate Visual Studio 2017 project files
 vs2019            Generate Visual Studio 2019 project files
 vs2022            Generate Visual Studio 2022 project files
````
#### 1.2) generate solution with your VS version
e.g. VS 2019
````shell
TCP_Srv/make$> premake5 vs2019
Building configurations...
Running action 'vs2019'...
Generated EchoSrv.sln...
Generated EchoSrv.vcxproj...
Done (42ms).
````
### 2) build executables in VS
- open solution (TCP_Srv/make/EchoSrv.sln)
- select a configuration (see also header of premake5.lua)
- build (and run)

### 3) VS keyboard shortcuts
- build: [CTRL] [shift] [B]
- (build and) run: [CTRL] [F5]
