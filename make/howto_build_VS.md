# Visual Studio build
## preconditions
- premake5
- Visual Studio

## 1) generate VS solution using premake5
### 1.1) find out suitable VS version using premake5 --help

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
### 1.2) generate solution with your VS version
e.g. VS 2019
````shell
TCP_Srv/make$> premake5 vs2019
Building configurations...
Running action 'vs2019'...
Generated TCP_Srv_Echo.sln...
Generated TCP_Srv_Echo.vcxproj...
Done (41ms)..
````
## 2) build executables in VS
- open solution (TCP_Srv/make/TCP_Srv_Echo.sln)
- select a configuration
    - _silent_: 
        - no debug
        - no output
        - optimized for speed
    - _verbose_:
        - verbose output
        - debug infos
        - no optimization
- build (and run)

## 3) VS keyboard shortcuts
- build: [CTRL] [shift] [B]
- (build and) run: [CTRL] [F5]
