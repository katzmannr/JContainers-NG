A toolchain for building JContainers NG on Linux
================================================

This toolchain file is for msvc-wine.

msvc-wine is a project that automates downloading of msvc for wine.

Assuming that you installed the basics to a folder like ~/msvc you
need to do a second run in the same folder.

As of the current release I used:

./vsdownload.py   --major 17   --msvc-version 17.14   --accept-license   \ 
--dest ~/msvc   --with-default yes   --with-workload yes   --with-msvc yes \
--with-sdk yes   --with-atl yes   --with-dia yes   --with-msbuild yes \
--with-asan yes   --architecture x64   --host-arch x64

The exact version depend on the one used for CI. As of document writing
it is a Windows 10 based MSVC 2022 toolchain (the project uses C++20).

Important: Later releases remove "stdext" extension from the project.
The setup for CommonLibSSE-NG requires older releases of dependencies.
Due to that stdext is a requirement until CommonLibSSE-NG is officially updated.

In addition JContainers NG was not updated for C++23. There could be many
issues due to deprecation of features or incompatibilities.

Luajit: The current release of Luajit is not building with msvc-wine. This
is because wine cmd is broken and fixes are going in very slow.

Windows cmd has dependencies deep into the kernel which is not part of wine.
wine uses the Linux kernel as base, so all functionality needs to be
reimplemented to match the Linux environment. For Luajit there are two
option: Rewrite the whole build system to CMake or use an artifact.

There are some cmake conversion, but I have found none with exact version
tracking, most integrate upates into their master branch over time.

This project seems to be the smallest (little dependencies) I found:

https://git.sr.ht/~lasse/luajit-cmake

