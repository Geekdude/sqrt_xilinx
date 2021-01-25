# Sqrt Xilinx OpenCL C Kernel

This repository is used to test the built-in sqrt() OpenCL C function inside a kernel targeting a Xilinx Alveo U250 FPGA.

Currently the kernel will fail to load using sw_emu, but will succeed when run using the hardware.

# To run the SW EMU test

Run the bash script `build_and_run_sw_emu.sh` after sourcing the Vitis tool chain.

The expected result is an error loading the kernel:

    Loading: './sw_emu/sqrt_kernel_v1.xclbin'
    ERROR: dlopen of /home/7ry/Data/xilinx/sqrt_xilinx/.run/88627/sw_emu/device0/binary_0/dltmp is failed. Please check undefined
    symbols in the kernel
    [XRT] ERROR: Failed to load xclbin.
    cv_opencl.cpp:47 Error calling cl::Program program(ctx, devices, bins, NULL, &err), error code is: -44


# To run the HW test

Run the bash script `build_and_run_hw.sh` after sourcing the Vitis tool chain.

The expected result is a successful run:
