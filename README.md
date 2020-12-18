# Restarter
Restarter (Restart Task) is a simple Win32 application (Windows subsystem) that continously launches an application specified in its arguments, wait for its termination, and repeats the cycle. Further arguments specified on the command line are passed to the launched executable.

Example:

```
rt.exe cmd.exe /k notepad.exe
```

The command launches Command Prompt, which in turn spawns Notepad and waits for it to finish. Afterwards, Command Prompt will exit and Restarter will repeat the cycle (relaunch Command Prompt, which opens Notepad and so on).

## Download

Binaries are available in [Releases](https://github.com/valinet/rt/releases).

## Compiling

The following prerequisites are necessary in order to compile this project:

- Microsoft C/C++ Optimizing Compiler - this can be obtained by installing either of these packages:

  - Visual Studio - this is a fully featured IDE; you'll need to check  "C/C++ application development role" when installing. If you do not  require the full suite, use the package bellow.
  - Build Tools for Visual Studio - this just installs the compiler,  which you'll be able to use from the command line

  Download either of those [here](http://go.microsoft.com/fwlink/p/?LinkId=840931). The guide assumes you have installed either Visual Studio 2019, either Build Tools for Visual Studio 2019.

Once you set up the prerequisites, to compile, either:

- Double click the WinCenterTitle.sln to open the solution in Visual  Studio. Choose Release and your processor architecture in the toolbar. Press [Ctrl] + [Shift] + [B] to compile.

- Open an "x86 Native Tools Command Prompt for VS 2019" (for x86), or "x64 Native Tools Command Prompt for VS 2019" (for x64) (search that in  Start), go to folder containing solution file and type:

  - For x86:

    ```
    msbuild WinCenterTitle.sln /property:Configuration=Release /property:Platform=x86
    ```

  - For x64:

    ```
    msbuild WinCenterTitle.sln /property:Configuration=Release /property:Platform=x64
    ```

  The resulting exe will be in "Release" folder (if you chose  x86), or "x64\Release" (if you chose x64) in the folder containing the  solution file.

* Open an "x86 Native Tools Command Prompt for VS 2019" (for x86), or "x64 Native Tools Command Prompt for VS 2019" (for x64) (search that in  Start), go to folder containing solution file and type:

  ```
  cd rt
  rc /nologo rt.rc
  cl /nologo /MT rt.c rt.res
  ```

  The resulting exe will be in the "rt" folder in the folder containing the solution file.

## License

[MIT License](LICENSE)

