#include <iostream>

#include "sqrt.hpp"
#include "cv_opencl.hpp"

// Host application structure:
// 1) Setting up the hardware
// 	- Retrieving the list of available *Xilinx platforms* (not other vendors)
// 	- Retrieving the list of devices supported by each Xilinx platform
// 	- Creating a context
// 	- Creating a program object from the pre-compiled FPGA binary (xclbin)
// 	- Creating a kernel object
// 2) Executing the kernels
// 3) Releasing the hw resources after the kernel returns
//

using std::cout;
using std::endl;

void usage()
{
    printf("usage: ./sqrt <array size> v<version> <build path>\n");
    printf("example: ./sqrt 100 v1 sw_emu\n");
}

int main(int argc, char** argv)
{
   // Keep track of Global variables by making a struct to store them in.

   // OpenCL variables
    CV_OpenCL cv_cl;
	// specific to xilinx
	std::string build_tgt;
    // My Additions
    std::vector<std::string> kernel_names{"compute_sqrt"};
	cv_cl.compute_sqrt_idx = 0;
    std::vector<cl::Event> iteration_events;
    cl::Event iteration_done;

	// from original code
	int version; // i.e., the kernel version 

    if (argc != 4){
        usage();
        exit(-1);
    }

	build_tgt = cv_cl.get_build_tgt(&argc, argv);	

	// note that this decrements argc by 1
	version = cv_cl.get_krnl_version(&argc, argv);

	// OPENCL HOST CODE AREA START
	// I did not use their OpenCL class. I created one that was inspired by their
	// original class.
	
    // ------------------------------------------------------------------------------------
    // Step 1: Get All PLATFORMS, then search for Target_Platform_Vendor (CL_PLATFORM_VENDOR)
    // Search for Platform: Xilinx 
    // Check if the current platform matches Target_Platform_Vendor
    // The get_devices function wraps the above steps 
    // ------------------------------------------------------------------------------------	
    // ------------------------------------------------------------------------------------
    // Step 1: Create Context; OCL_CHECK wraps some error checking
    // ------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------
    // Step 1: Create Command Queue
    // ------------------------------------------------------------------------------------
    // ------------------------------------------------------------------
    // Step 1: Load Binary File from disk
    // ------------------------------------------------------------------		
    // -------------------------------------------------------------
    // Step 1: Create the program object from the binary and program the FPGA device with it
    // -------------------------------------------------------------	
    // -------------------------------------------------------------
    // Step 1: Create Kernels
    // -------------------------------------------------------------
    // all of this is handled by PV_OpenCL::init
	cv_cl.init(version, build_tgt, kernel_names);

    // ================================================================
    // Step 2: Setup Buffers and run Kernels
    // ================================================================
    //   o) Allocate Memory to store the results 
    //   o) Create Buffers in Global Memory to store data
    // ================================================================

    // ------------------------------------------------------------------
    // Step 2: Create Buffers in Global Memory to store data
    //             o) d_gpu_wall 
    //             o) d_gpu_result[0] 
    //             o) d_gpu_result[1]
    // ------------------------------------------------------------------	

    // .......................................................
    // Allocate Global Memory for source_in1
    // .......................................................	

    // ------------------------------------------------------
    // Step 2: Copy Input data from Host to Global Memory on the device
    // ------------------------------------------------------

    // ----------------------------------------
    // Step 2: Submit Kernels for Execution
    // ----------------------------------------

    // --------------------------------------------------
    // Step 2: Copy Results from Device Global Memory to Host
    // --------------------------------------------------

    // ============================================================================
    // Step 3: Release Allocated Resources
    // ============================================================================

    // All of this handled by the sqrt class.
    SQRT sqrt {cv_cl, atoi(argv[1])};

    cout << "Yay Success!" << endl;
    return 0;
}

