# Zuma Deluxe Decompilation

Based on reverse engineering of Mac and Windows executables distributed on PopCap.com.

Status: Fully playable, but potential bugs and inaccuracies are expected.

### Dependencies

- It's pretty much self contained, it compiles and works fine with MinGW and MSVC toolchains.
- CMake 3.18+: https://cmake.org/
- BASS 1.8 DLL, shipped with original game, if you're compiling a 32-bit version.
- or BASS 2.4 DLL, which you can get from https://www.un4seen.com/, required for 64-bit builds 
  (as BASS 1.x is not available for 64-bit platforms), optionally supported in 32-bit builds.

### Differencies / accuracy notes

- The most noticeable difference is that this project is based on open-source release of SexyAppFramework (1.3), with patches to bring back certain old behavior. Later we may reverse engineer framework code and bring it closer to older/internal versions of SexyApp.
- To reduce legal risk, we do not aim to reverse engineer DRM stuff, the resulting executable is DRM-free.
- Game logic code is where we put most care and aim for asm accuracy where possible.
- Certain variable/parameter names and missing methods have been reconstructed by reverse engineering Flash/C# ports of Zuma's Revenge and Zuma web game.

### Compiling

The easiest way to compile the project is using the `build.bat` or `build.sh` scripts, which will download MSVC 7.0 build tools, CMake, Ninja and compile the project for you. The compiled .exe can be found in `build/source/CircleShoot/Zuma.exe`.

The script downloads a small, self-contained SDK with a subset of tools extracted from Visual Studio .NET 2002 (MSVC 7.0) installation - the exact same version according to the data in Rich header of the original .exe.

Note that this restricts you to the ancient toolchain, which is primarily used for comparing the assembly output of the compiled binary to the original executable. You can also use modern toolchains like MSVC 2019/2022 or MinGW-w64, by running CMake manually and specifying the desired generator.

The `bass.dll` file is not included in the repository. Please either copy it from the original game installation (along with data files), or download it from the website linked above - in case you are building a 64-bit version.

It is also possible to use the Visual Studio .NET 2002 solution in `source/CircleShoot/CircleShoot.vcproj` - however it is not recommended. Using CMake and Ninja has significant advantages, including support for parallel compilation (on modern 16+ core CPUs gives a ~30x speedup), support for modern tooling and serves as a better starting point for any future ports - all while making it possible to still use the ancient toolchain for reference purposes.

### The executables used for comparison/reference

- `51cf7bbc6df4a41ec137a52d4496c3149ba32c9ae2acc4653279d04c164809aa  Zuma.exe`
  
  from official web release / PopCap Ultimate Collection CD.
- `35bcba57fdd7e8eed49c51b4b44d9b6565547a7b24ad2802d19ae0982dbdda3f  Zuma.decrypted`
  
  `3a40ff3ed0d8533f07c417451bf2fd0a3751d70db6b7e207504430f6943a9966  Zuma Deluxe.payload`
  
  OS X version 1.0.0 from PopCap.com (PowerPC + x86 universal) - after descrambling the `Zuma Deluxe.payload` file:

  ```shell
  python -c "d=open('Zuma Deluxe.payload','rb').read(); open('Zuma.decrypted','wb').write(b''.join(d[i:i+16][::-1] for i in range(0,len(d),16)))"
  ```

### Miscellaneous notes

Legal notes:
- The source code of game itself (everything in `source/CircleShootApp/`) is provided as-is.
- Any modifications are licensed under MIT.
- The main focus of the project is bug-fixing, modding and porting and is done in a good faith - we don't condone 
  piracy, we just essentially want to give the game a "second life".
- The decompilation project does not include any assets - you need to get them from your own copy of the game - if 
  you don't own it, Steam version is easily available: https://store.steampowered.com/app/3330/Zuma_Deluxe/
