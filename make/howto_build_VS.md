# Visual Studio build
## preconditions
- premake5 available
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
$> premake5 vs2019
Building configurations...
Running action 'vs2019'...
Generated TCP_Srv_Base.sln...
Generated TCP_Srv_Base.vcxproj...
Generated sample_Method_Threads.sln...
Generated sample_Method_Threads.vcxproj...
Generated sample_Srv_MicroSoft_VS.sln...
Generated sample_Srv_MicroSoft_VS.vcxproj...
Generated sample_Srv_Tenouk.sln...
Generated sample_Srv_Tenouk.vcxproj...
Done (49ms).
````
## 2) build executables in VS
- open solution (*.sln)
- build and run

## 3) VS keyboard shortcuts
- build: [CTRL] [shift] [B]
- (build and) run: [CTRL] [F5]
