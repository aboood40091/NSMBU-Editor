# NSMBU-Editor
Editor for many formats present in NSMBU.  
Only runs on Windows at the moment.  

## Supported formats
* PTCL (effects file, currently viewing only)

## Dependencies
* GCC  
* GLEW
* GLFW
* OpenGL 4.4 or higher

## Building
(TODO: Make a Makefile)  
* Need to define the following macro: `EFT_WIN`.  
* Add `include` and `include/win` to your header paths.  
* Build using GCC flag `-m32` (32-bit).
