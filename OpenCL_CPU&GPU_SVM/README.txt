Student Name: Steven Smith
Student ID: 40057065

Development Environment:
OS: Ubuntu 20.04
Kernel Version: 5.6
CPU: Ryzen 7 2700 8C16T @4Ghz
Memory: 16 GB 
GPU: AMD Radeon RX 5700 8Gb Vram
Compiler: GCC 10.2

Information: 

The assignment was programmed using OpenGL and diffrerent libraries than what was used in the labs. The VCPKG folder in the Framework directory
contains the proper linux x86_64 version of the tools used. vcpkg can be setup on windows, but I have personnally encountered issues with its version of glm.
Furthermore, the application uses a CMake file and was programmed in CLion.

The program will work no problems on a Linux operating system by copy pasting the directory somewhere and opening it in an IDE that supports CMake projects.
You will also need to link to the vcpkg directory for your compiler. 

If you use CLion just copy&paste:
	-DCMAKE_TOOLCHAIN_FILE=../Framework/vcpkg-2020.01/scripts/buildsystems/vcpkg.cmake

into the CMake options field. 


Furthermore, it is important to note, that the project uses Shared Virtual Memory which I believe requires OpenCL 2.0 or greater support. From what I know
only intel hd graphics, nvidia RTX 3000 series and AMD rDNA gpu have software support for version 2.0 or greater.

My personal configuration uses pocl to gain access to my cpu as a valid openCL target, however, the program should fetch the first avaialble CPU from any platform. 
