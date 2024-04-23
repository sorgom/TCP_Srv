# how to build
## gcc build
### preconditions
- make
- gcc
### use build.sh
call help
````shell
TCP_Srv/make$> ./build.sh -h
````
then call to build, e.g.
````shell
TCP_Srv/make$> ./build.sh -c
````

## Visual Studio build
### preconditions
- premake5
- Visual Studio

### 1) generate VS solution using premake5
- find out suitable VS version using premake5 --help

````shell
TCP_Srv\make> premake5 --help
Usage: premake5 [options] action [arguments]
...
ACTIONS
 vs2005            Generate Visual Studio 2005 project files
 ...
 vs2022            Generate Visual Studio 2022 project files
````
- generate solution with your VS version, e.g. VS 2019
````shell
TCP_Srv\make> premake5 vs2019
````
### 2) build executables in VS
- open solution (TCP_Srv/make/EchoSrv.sln)
- select a configuration (see also header of [premake5.lua](premake5.lua))
- build (and run)

### 3) VS keyboard shortcuts
- build: [CTRL] [shift] [B]
- (build and) run: [CTRL] [F5]
