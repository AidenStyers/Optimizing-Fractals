# How to Run Code

First open x64 native tools command prompt, then run   cmake -S . -B build -G "Visual Studio 17 2022" -A x64 
Make sure you are in the correct directory. Then in the VS Code terminal you can run   cmake --build build --config Release   to build the .dll file in the Release directory.