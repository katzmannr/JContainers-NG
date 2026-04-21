[![Build status](https://ci.appveyor.com/api/projects/status/r8kex3xnxmktn1sq?svg=true)](https://ci.appveyor.com/project/ryobg/jcontainers)
[![Latest release](https://img.shields.io/github/release/ryobg/jcontainers/all.svg)](https://github.com/ryobg/jcontainers/releases)

<img src="logo.png?raw=true" height="256">

# JContainers NG (64-bit)

A project to extend [Skyrim's Papyrus
scripting](https://www.creationkit.com/index.php?title=Category:Papyrus) with
[JSON formatted](https://json.org/) serializable data structures.

> **Important**
>
> This project is fork of the original [JContainers](https://github.com/SilverIce/JContainers). It
> strives to convert and mash it up to the new Skyrim Special 64-bit edition. Cudos to the original
> author and all of his supporters!
>
> JContainers NG is concentrating on simplifying the build and distribution
> from [ryobgs fork](https://github.com/ryobg/JContainers/fork) by using CommonLibSSE-NG and removing
> most dependencies from submodules 
> CMake can resolve the dependencies or vcpkg be used as a dependency package manager

### Why?

If you are programmer, sooner or later you'll notice the lack of many useful features in Papyrus.
There is no way to:

- Append or erase values from arrays
- Put an array into an array (i.e. no nested arrays)
- Put multiple value types into a single array
- Have associative containers 
- Be able to load or save a data into a file

### Solution

Since there is no source code of the Papyrus virtual machine, it is tricky and no easy to extend the
existing Papyrus Array type or add new data structure types. JContainers implements from scratch its
own data structures, garbage collector and other infernals. Features offered:

- Data structures: arrays and associative containers (a.k.a. maps or dictionaries)
- Import and export data to and from JSON files
- Embedded, lightweight scripting with [Lua](https://www.lua.org/)
- Interaction with JContainers via C++ interface.

### Full documentation

[Latest documentation](https://github.com/ryobg/jcontainers/wiki)

### Can I help?

Sure! Feel free to do whatever you think is good - post feature requests, report bugs, improve Wiki
or source code.

# Building from source

### Prerequisites

* [Microsoft Visual Studio Free](https://www.visualstudio.com/downloads/) 
  Community Edition with Visual C++ support would suffice. All project files are of that version,
  but with a bit of manual work they may be converted to older versions too (e.g. 2013, though
  issues most certainly will arrise). If you want a leight weight IDE instead use
* [Visual Studio Code](https://code.visualstudio.com/download)
  Free and built on open source. Integrated Git, debugging and extensions. Runs on other platforms
  like Ubuntu or Mac too. C/C++ support for Visual Studio Code is provided by a Microsoft C/C++ 
  extension to enable cross-platform C and C++ development on Windows, Linux, and macOS. Only
  g++, clang, xcode are supported by this extension. The VC Compiler can no longer be installed
  separately from Visual Studio Community Edition (Build Tools have been removed). Some Installers
  allow you to install the VC Compiler without the IDE, but for me this option didn't work.
* A [Python](https://www.python.org/downloads/) environment for Windows, version 3.4 or later.
  This is needed to run some helper scripts for testing, building distributions and any other small
  helpful tasks. Its `python.exe` should be available on the PATH variable.
* The GIT revisioning system and/or GitHub account may help if you want to contribute or work more
  easily with the public repository of this project.

### First time setup

1. Run `git submodule update --init --recursive` so that CommonLibSSE NG dependency gets downloaded 
   and linked to the project folder.
2. If you have vcpkg installed, use it to install the preinstalled boost package:
   cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
3. The SKSE CommonLibSSE-NG is already in the correct format, so no manual build steps required.
4. Open the `CMakeLists.txt` file with Visual Studio. The solution is created from the CMake
   extension. Rebuild the whole solution. It will take some time. 
5. After successfull build, run from the command line `python tools\install.py x64\Release 64`.
   Eventually swap `Release` for `Debug` - depending on what kind of distribution was build and
   actually is wanted in the `dist\` folder.
6. Optionally, run `python tools\test.py x64\Release\Data\SKSE\Plugins\JContainers64.dll`. Again it
   depends whether `Release` or `Debug` builds should be tested.

### Building from github CI

If you do not want to download build tools you can run a test build from CI. You need to create
a merge request for that. The projects comes with github action definition. After creating the
request you can run it to create a test build from your changes.

### Merging changes into this repository

I can merge changes to this repository. The drawback is though that the maintainers will
not see the changes you want to contribute. Because of that I recommend you to create a
merge request on the original github page from [JContainer 64](https://github.com/ryobg/JContainers/fork)

### Use of other IDE

While the team does only support VisualStudio, other IDE that integrate VS Compiler have
been reported to be usable too, f.e. QtCreator. Other IDE usually do not convert the CMake
project so the build is done in a similar way as on the CI (github actions).

That's it!

