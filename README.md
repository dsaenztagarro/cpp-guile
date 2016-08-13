# References

- [Debugging with GDB](https://www.sourceware.org/gdb/current/onlinedocs/gdb.html)
- [GNU Guile 2.0.12 Reference Manual](https://www.gnu.org/software/guile/manual/)
- [The LLVM Compiler Infrastructure](http://llvm.org)
- [Clang 4.0 documentation](http://clang.llvm.org/docs/index.html)
- [FSEvents C Example](http://stackoverflow.com/questions/11556545/fsevents-c-example)

- [Unit testing C Programs with Mock functions](http://blogs.grammatech.com/unit-testing-c-programs-with-mock-functions)
- [Doing C code unit testing on a shoestring: Part 1- The basics and the tools](http://www.embedded.com/design/programming-languages-and-tools/4007177/2/Doing-C-code-unit-testing-on-a-shoestring-Part-1-The-basics-and-the-tools)
- [Doing C code unit testing on a shoestring: Part 2- Code coverage analysis](http://www.embedded.com/design/prototyping-and-development/4007183/2/Doing-C-code-unit-testing-on-a-shoestring-Part-2-Code-coverage-analysis)
- [Doing C code unit testing on a shoestring: Part 3 - Building a unit test framework](http://www.embedded.com/design/prototyping-and-development/4007187/2/Doing-C-code-unit-testing-on-a-shoestring-Part-3--Building-a-unit-test-framework)

Find information from:

- `man dirent`. Search modules and c functions.
- Inspecting includes files: `/usr/include/dirent.h`, `/usr/include/sys/dirent.h`.


### Codesign gdb on Mac OS X Yosemite (10.10.2)


Mac OS X 10.10.2 does not come with gdb pre installed. It is available on homebrew:

```
$ brew tap homebrew/dupes
$ brew install gdb
```

The binary is installed on /usr/local/bin

When initializing gdb on a program (a.out) it will produce the following error:

```
Starting program: a.out
Unable to find Mach task port for process-id XXXXX: (os/kern) failure (0x5).
 (please check gdb is codesigned - see taskgated(8))
```

Start Keychain Access application (/Applications/Utilities/Keychain Access.app)
This error occurs because OSX implements a pid access policy which requires a digital signature for binaries to access other processes pids. To enable gdb access to other processes, we must first code sign the binary. This signature depends on a particular certificate, which the user must create and register with the system.

To create a code signing certificate, open the Keychain Access application. Choose menu Keychain Access -> Certificate Assistant -> Create a Certificate…

Choose a name for the certificate (e.g., gdb-cert), set Identity Type to Self Signed Root, set Certificate Type to Code Signing and select the Let me override defaults. Click several times on Continue until you get to the Specify a Location For The Certificate screen, then set Keychain to System.

Double click on the certificate, open Trust section, and set Code Signing to Always Trust. Exit Keychain Access application.

Restart the taskagted service, and sign the binary.

```
$ sudo killall taskgated
$ codesign -fs gdb-cert /usr/local/bin/gdb
```
