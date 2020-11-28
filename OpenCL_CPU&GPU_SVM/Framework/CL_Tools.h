//
// Created by amadeus on 2020-10-31.
//
#define __CL_ENABLE_EXCEPTIONS

#ifndef OPENCL_CL_TOOLS_H
#define OPENCL_CL_TOOLS_H

#include <CL/cl2.hpp>
#include <iostream>
void printAvailablePlatformInfo(){
    try
    {
        // Discover number of platforms
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        std::cout << "\nNumber of OpenCL plaforms: " << platforms.size() << std::endl;

        // Investigate each platform
        std::cout << "\n-------------------------" << std::endl;
        for (std::vector<cl::Platform>::iterator plat = platforms.begin(); plat != platforms.end(); plat++)
        {
            std::string s;
            cl_device_type type;
            plat->getInfo(CL_PLATFORM_NAME, &s);
            std::cout << "Platform: " << s << std::endl;

            plat->getInfo(CL_PLATFORM_VENDOR, &s);
            std::cout << "\tVendor:  " << s << std::endl;

            plat->getInfo(CL_PLATFORM_VERSION, &s);
            std::cout << "\tVersion: " << s << std::endl;

            // Discover number of devices
            std::vector<cl::Device> devices;
            plat->getDevices(CL_DEVICE_TYPE_ALL, &devices);
            std::cout << "\n\tNumber of devices: " << devices.size() << std::endl;

            // Investigate each device
            for (std::vector<cl::Device>::iterator dev = devices.begin(); dev != devices.end(); dev++ )
            {
                std::cout << "\t-------------------------" << std::endl;

                dev->getInfo(CL_DEVICE_TYPE, &type);
                std::cout << "\t\tName: " << type << std::endl;

                dev->getInfo(CL_DEVICE_OPENCL_C_VERSION, &s);
                std::cout << "\t\tVersion: " << s << std::endl;

                int i;
                dev->getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &i);
                std::cout << "\t\tMax. Compute Units: " << i << std::endl;

                size_t size;
                dev->getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &size);
                std::cout << "\t\tLocal Memory Size: " << size/1024 << " KB" << std::endl;

                dev->getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &size);
                std::cout << "\t\tGlobal Memory Size: " << size/(1024*1024) << " MB" << std::endl;

                dev->getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &size);
                std::cout << "\t\tMax Alloc Size: " << size/(1024*1024) << " MB" << std::endl;

                dev->getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &size);
                std::cout << "\t\tMax Work-group Total Size: " << size << std::endl;

                std::vector<size_t> d;
                dev->getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &d);
                std::cout << "\t\tMax Work-group Dims: (";
                for (std::vector<size_t>::iterator st = d.begin(); st != d.end(); st++)
                    std::cout << *st << " ";
                std::cout << "\x08)" << std::endl;

                std::cout << "\t-------------------------" << std::endl;

            }

            std::cout << "\n-------------------------\n";
        }

    }
    catch (cl::Error err)
    {
        std::cout << "Check cl.h for error codes." << std::endl;
        exit(-1);
    }
}

cl::Platform GetDefaultPlatform(){
    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    return default_platform;
}

cl::Device GetDefaultDevice(cl::Platform *default_platform){
    std::vector<cl::Device> all_devices;
    default_platform->getDevices(CL_DEVICE_TYPE_CPU, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];

    return default_device;
}

int TestDevices(){
    //get all platforms (drivers)

    cl::Platform default_platform=GetDefaultPlatform();
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    cl::Device default_device= GetDefaultDevice(&default_platform);
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";


    cl::Context context({default_device});

    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    std::string kernel_code=
            "   void kernel simple_add(global const float3* A, global const float3* B, global int* C){       "
            "       int value = 0;    "
            "       if(A[get_global_id(0)].x == B[get_global_id(0)].x && "
            "           A[get_global_id(0)].y == B[get_global_id(0)].y &&"
            "           A[get_global_id(0)].z == B[get_global_id(0)].z ){"
            "           value = 1;    "
            "       }"
            "       C[get_global_id(0)]=value;                 "
            "   }                                                                               ";
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }


    // create buffers on the device
    cl::Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(cl_float3)*10);
    cl::Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(cl_float3)*10);
    cl::Buffer buffer_C(context,CL_MEM_READ_WRITE,sizeof(int)*10);

    cl_float3 A[] = {cl_float3{0,1,0} , cl_float3{1,0,0}, cl_float3{2,0,0}, cl_float3{3,0,0}, cl_float3{4,0,0}, cl_float3{5,0,0}, cl_float3{6,0,0}, cl_float3{7,0,0}, cl_float3{8,0,0}, cl_float3{9,0,0}};
    cl_float3 B[] = {cl_float3{0,0,0}, cl_float3{1,0,0}, cl_float3{2,0,0}, cl_float3{0,0,0}, cl_float3{1,0,0}, cl_float3{2,0,0}, cl_float3{0,0,0}, cl_float3{1,0,0}, cl_float3{2,0,0}, cl_float3{0,0,0}};
    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(cl_float3)*10,A);
    queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(cl_float3)*10,B);

    //alternative way to run the kernel
    cl::Kernel kernel_add=cl::Kernel(program,"simple_add");
    kernel_add.setArg(0,buffer_A);
    kernel_add.setArg(1,buffer_B);
    kernel_add.setArg(2,buffer_C);
    queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    queue.finish();

    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);

    std::cout<<" result: \n";
    for(int i=0;i<10;i++){
        std::cout<<C[i]<<" ";
    }

    return 0;
}

cl::Program buildProgram(cl::Context context, cl::Device device, string file){
    // read file
    std::ifstream programFile(file);
    // read file
    std::string kernel_code(std::istreambuf_iterator<char>(programFile),
                              (std::istreambuf_iterator<char>()));

    if(kernel_code.empty()){
        std::cout<<" Error building: source file does not exist\n";
        exit(1);
    }
    // create source
    cl::Program::Sources source;
    source.push_back({kernel_code.c_str(),kernel_code.length()});
    // create program
    cl::Program program(context,source);
    // build
    if(program.build({device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)<<"\n";
        exit(1);
    }

    return program;
}

cl::Device GetDeviceTarget(cl_device_type targetDevice){
    try
    {
        // Discover number of platforms
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        // Investigate each platform
        for (std::vector<cl::Platform>::iterator plat = platforms.begin(); plat != platforms.end(); plat++)
        {
            std::vector<cl::Device> devices;
            plat->getDevices(CL_DEVICE_TYPE_ALL, &devices);
            cl_device_type deviceType;
            // Investigate each device
            for (std::vector<cl::Device>::iterator dev = devices.begin(); dev != devices.end(); dev++ )
            {
                dev->getInfo(CL_DEVICE_TYPE, &deviceType);
                if(deviceType == targetDevice){
                    return *dev;
                }
            }
        }
        std::cout << "Device does not exist on any platform" << std::endl;
        exit(-1);
    }
    catch (cl::Error err)
    {
        std::cout << "Check cl.h for error codes." << std::endl;
        exit(-1);
    }
}
#endif //OPENCL_CL_TOOLS_H
