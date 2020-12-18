# Restarter
Restarter (Restart Task) is a simple Win32 application (Windows subsystem) that continously launches an application specified in its arguments, wait for its termination, and repeats the cycle. Further arguments specified on the command line are passed to the launched executable.

Example:

```
rt.exe cmd.exe /k notepad.exe
```

The command launches Command Prompt, which in turn spawns Notepad and waits for it to finish. Afterwards, Command Prompt will exit and Restarter will repeat the cycle (relaunch Command Prompt, which opens Notepad and so on).

## Download

Binaries are available in [Releases](https://github.com/valinet/rt/releases).

There usually are 2 versions included:

* rt.exe - standard executable

* rts.exe - standard executable, but a sleep happens before the executable is launched again; to adjust the sleep interval (the default is 999ms which is approximately 1 second), launch the script "adjsleep.ps1" from a command prompt window like this:

  ```
  powershell -ExecutionPolicy Bypass -File adjsleep.ps1 ..\x64\Release\rts.exe 500 0x6D5
  ```

  * Argument 1 ($path) [`..\x64\Release\rts.exe`] is the path to the rt executable with sleep

  * Argument 2 ($value) [`500`] is the new value (how long to sleep before relaunching)

  * Argument 3 ($offset) [`0x6D5`] is offset in the binary where the value is value that was supplied at compile time is written. `0x6D5` is valid for version 1.0.0.1 of the application.

    To determine this value, open the executable file in [HxD](https://mh-nexus.de/en/hxd/), click the Search menu - Find, choose "Hex-values", type "e703", for search direction choose "All" and click "Search all". If done correctly, a single entry will be shown in the bottom part of the window. The value you are interested in (the offset to supply to the script) is available in the "Offset" column. Usually, you can find the right value in instructions published with each release on its page.

  To reproduce rts.exe when compiling, uncomment the line (remove leading `//`) containing `Sleep(999);` in `rt.c` (use Find to quickly locate it).

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

