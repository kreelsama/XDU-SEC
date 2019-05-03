If you want to use this repository, you must compile *libmiracl* exclusively.

The `Cmakelists` files require further modifications to adjust your compiled static libs.

The files for miracl are included in `./miracl/` folder, which you can use directly to compile your own static libs.

I recommend using Linux and Cygwin or MinGW in Windows rather than Visual Studio.

The easier way to do is just to replace `libmiracl_linux.a` with the lib you compiled and the modify the `CMakeLists` files.