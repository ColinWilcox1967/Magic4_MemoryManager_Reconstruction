# Magic4_MemoryManager_Reconstruction
A reconstruction of my C-based in-memory memory manager written for Magic4 in 2000. Written in Microsoft C99.

Used in many early phones from Motorola, Philips, Ericsson and others back in the day as well as other embedded devices back in the day as part of the Magic4 messaging suite.


- **Compile the Source Code into an Object File**

  Run the following command to compile the _mem_manager.c_ file into an object file (.obj). The /c flag tells the compiler to only compile and not link.

  **cl /c /std:c99 mem_manager.c**

  This will create _mem_manager.obj_ (the object file) in the same directory.

- **Create the Static Library**
  Next, use the lib tool to create a static library (_memmanager.lib_) from the object file. Run the following command:

  **lib /out:memmanager.lib mem_manager.obj**


- **Compile and Link Your Application**

  If you have a _main.c_ file or any other source file that uses the memory manager, compile and link it along with the static library.

  For example, to compile and link a file main.c into an executable my_application.exe, run the following command:

  **cl /std:c99 main.c memmanager.lib /Fe:my_application.exe**

- **Run Your Application**

  After successful compilation and linking, you can run the executable.

  _my_application.exe_

- **Debugging and Flags**

  If you want to include debugging symbols in your executable, you can add the /Zi flag to the compile command and /DEBUG to the linking step:

  **cl /c /std:c99 /Zi main.c**
  **lib /out:memmanager.lib mem_manager.obj**
  **cl /std:c99 /DEBUG main.c memmanager.lib /Fe:my_application.exe**
