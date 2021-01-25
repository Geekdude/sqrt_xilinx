# Sqrt Xilinx OpenCL C Kernel

This repository is used to test the built-in sqrt() OpenCL C function inside a kernel targeting a Xilinx Alveo U250 FPGA.

Currently the kernel will fail to load using sw_emu, but will succeed when run using the hardware.

# To run the SW EMU test

Run the bash script `build_and_run_sw_emu.sh` after sourcing the Vitis tool chain.

The expected result is an error loading the kernel:

    $ ./build_and_run_sw_emu.sh
    Grab build target from argv
    arg_idx = 3
    argv[3] = sw_emu
    Build tgt was found to be sw_emu
    argc now = 3
    Grab kernel version number
    arg_idx = 2
    argv[2] = v1
    Using version 1
    argc now = 2
    XRT build version: 2.7.766
    Build hash: 19bc791a7d9b54ecc23644649c3ea2c2ea31821c
    Build date: 2020-08-17 23:51:58
    Git branch: 2020.1_PU1
    PID: 46385
    UID: 40412
    [Mon Jan 25 23:01:37 2021 GMT]
    HOST: pcie
    EXE: /home/7ry/Data/xilinx/sqrt_xilinx/sqrt
    [XRT] WARNING: The data_transfer_trace setting of coarse is not supported in emulation. Fine will be used.
    Found Platform
    Platform Name: Xilinx
    hi starting cmd q
    0
    hi done with cmd q
    my_kernel_name = ./sw_emu/sqrt_kernel_v1
    binary_name = ./sw_emu/sqrt_kernel_v1.xclbin
    INFO: Reading ./sw_emu/sqrt_kernel_v1.xclbin
    Loading: './sw_emu/sqrt_kernel_v1.xclbin'
    ERROR: dlopen of /home/7ry/Data/xilinx/sqrt_xilinx/.run/46385/sw_emu/device0/binary_0/dltmp is failed. Please check undefined symbols in the kernel
    [XRT] ERROR: Failed to load xclbin.
    cv_opencl.cpp:47 Error calling cl::Program program(ctx, devices, bins, NULL, &err), error code is: -44


# To run the HW test

Run the bash script `build_and_run_hw.sh` after sourcing the Vitis tool chain.

The expected result is a successful run:

    $ ./build_and_run_hw.sh
    Grab build target from argv
    arg_idx = 3
    argv[3] = hw
    Build tgt was found to be hw
    argc now = 3
    Grab kernel version number
    arg_idx = 2
    argv[2] = v1
    Using version 1
    argc now = 2
    Found Platform
    Platform Name: Xilinx
    hi starting cmd q
    0
    hi done with cmd q
    my_kernel_name = ./hw/sqrt_kernel_v1
    binary_name = ./hw/sqrt_kernel_v1.xclbin
    INFO: Reading ./hw/sqrt_kernel_v1.xclbin
    Loading: './hw/sqrt_kernel_v1.xclbin'
    XRT build version: 2.7.766
    Build hash: 19bc791a7d9b54ecc23644649c3ea2c2ea31821c
    Build date: 2020-08-17 23:51:58
    Git branch: 2020.1_PU1
    PID: 42650
    UID: 40412
    [Mon Jan 25 22:56:23 2021 GMT]
    HOST: pcie
    EXE: /home/7ry/Data/xilinx/sqrt_xilinx/sqrt
    [XRT] WARNING: unaligned host pointer '0xe78cc0' detected, this leads to extra memcpy
    Starting...
    Computation Duration: 0.000568077 s
    Result: sqrt(25) = 5
    Saving solution...
    Saved solution to results.txt...
    Done...
