#include "cv_opencl.hpp"

CV_OpenCL::~CV_OpenCL()
{
}

/**
 * Initialization function.
 *   Wraps:
 *     GetDevices()
 *	   BuildKernel()
 */
void CV_OpenCL::init(int version, const std::string build_tgt,
	const std::vector<std::string> names)
{
	this->version = version;
	GetDevices();

	devices.resize(1); //Isolating the first Xilinx FPGA enumerated
	cl::Device device(devices[0]);

	ctx = cl::Context(device, NULL, NULL, NULL, &err);
	OCL_CHECK_NO_CALL(err, "creating ctx");
	std::cout << "hi starting cmd q" << std::endl;
	cmd_q = cl::CommandQueue(ctx, device, CL_QUEUE_PROFILING_ENABLE, &err);
	OCL_CHECK_NO_CALL(err, "creating cmd_q");
	std::cout << err << std::endl;
	std::cout << "hi done with cmd q" << std::endl;

	//buildKernel(version)
	//getVersionName

	std::string my_kernel_name(
		std::string("./") + build_tgt + std::string("/sqrt_kernel_v")
		+ std::to_string(version)
	);
	std::string binary_name(
		my_kernel_name
		+ std::string(".xclbin")
	);
	std::cout << "my_kernel_name = " << my_kernel_name << std::endl;
	std::cout << "binary_name = " << binary_name << std::endl;

	cl::Program::Binaries bins;
	BuildKernel(binary_name, bins);

	OCL_CHECK(err, cl::Program program(ctx, devices, bins, NULL, &err));

	compute_sqrt_idx = 0;
	for (auto& name : names)
	{
		OCL_CHECK(err, cl::Kernel krnl_dynproc(program, name.c_str(), &err));
		str_kernel_map[name.c_str()] = krnl_dynproc;

		kernel_names.push_back(name);
		kernels.push_back(krnl_dynproc);
	}

	// Memory cleanup for the variable used to hold the kernel source.
	for (int i = 0; i < (int)bins.size(); ++i)
		delete[] const_cast<char *>(static_cast<const char*>(bins[i].first));
}

void CV_OpenCL::GetDevices()
{
	std::vector<cl::Platform> platforms;
	OCL_CHECK(err, err = cl::Platform::get(&platforms));
	cl::Platform platform;
	for (size_t i  = 0 ; i < platforms.size(); i++){
		platform = platforms[i];
		OCL_CHECK(err, std::string platformName = platform.getInfo<CL_PLATFORM_NAME>(&err));
		if (platformName == "Xilinx"){
			std::cout << "Found Platform" << std::endl;
			std::cout << "Platform Name: " << platformName.c_str() << std::endl;
			break;
		}
	}

	OCL_CHECK(err, platform.getDevices(CL_DEVICE_TYPE_ALL, &devices));
}

void CV_OpenCL::BuildKernel(std::string binary_name,
	cl::Program::Binaries& bins)
{
	unsigned int file_buf_size;
	char *file_buf = read_binary_file(binary_name, file_buf_size);
	bins = {{file_buf, file_buf_size}};

	// TODO: figure out a way to free file_buf
}

std::string CV_OpenCL::get_build_tgt(int *argc, char **argv)
{
	char build_tgt[8]; 
	int arg_idx = *argc - 1;

	std::cerr << "Grab build target from argv" << std::endl;
	std::cerr << "arg_idx = " << arg_idx << std::endl;
	std::cerr << "argv[" << arg_idx << "] = " << argv[arg_idx] << std::endl;
	if (arg_idx > 0)
		std::sscanf(argv[arg_idx], "%8s", build_tgt);

	std::string ret_build_tgt(build_tgt);
	std::cerr << "Build tgt was found to be " << ret_build_tgt << std::endl;

	*argc -= 1;
	std::cerr << "argc now = " << *argc << std::endl;
	return std::string(build_tgt);
}

int CV_OpenCL::get_krnl_version(int *argc, char **argv)
{
	int v_num = 0;
	int shift = 0;
	int arg_idx = *argc - 1;

	std::cerr << "Grab kernel version number" << std::endl;
	std::cerr << "arg_idx = " << arg_idx << std::endl;
	std::cerr << "argv[" << arg_idx << "] = " << argv[arg_idx] << std::endl;
	if (arg_idx > 0)
	{
		int ret = std::sscanf(argv[arg_idx], "v%d", &v_num);
		if (ret == 1)
			++shift;
	}
	std::cerr << "Using version " << v_num << std::endl;

	*argc -= shift;
	std::cerr << "argc now = " << *argc << std::endl;
	return v_num;
}

// returns number of platforms found
void CV_OpenCL::display_device_info(std::vector<cl::Platform> &platforms)
{
	cl_int err;
	
	// Get all platforms
	OCL_CHECK(err, err = cl::Platform::get(&platforms));
	std::cout << "num platforms = " << platforms.size() << std::endl;;

	fprintf(stderr, "\nQuerying devices for info:\n");
	for (std::vector<cl::Platform>::iterator it1 = platforms.begin(); 
		it1 < platforms.end(); ++it1)
	{
		cl::Platform platform(*it1);
		std::cout 
			<< "Platform Name: " 
			<< platform.getInfo<CL_PLATFORM_NAME>() 
			<< std::endl;
		std::cout 
			<< "Platform Vendor: " 
			<< platform.getInfo<CL_PLATFORM_VENDOR>() 
			<< std::endl;

		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

		for(std::vector<cl::Device>::iterator it2 = devices.begin();
			it2 < devices.end(); ++it2)
		{
			cl::Device device(*it2);
			std::cout 
				<< "\t\tDevice Vendor: " 
				<< device.getInfo<CL_DEVICE_VENDOR>() 
				<< std::endl;
			std::cout 
				<< "\t\tDevice Name: " 
				<< device.getInfo<CL_DEVICE_NAME>() 
				<< std::endl;  
			std::cout 
				<< "\t\tDevice Version: " 
				<< device.getInfo<CL_DEVICE_VERSION>()
				<< std::endl;  
			std::cout 
				<< "\t\tDevice Global Memory: " 
				<< device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() 
				<< std::endl;
			std::cout 
				<< "\t\tDevice Local Memory: " 
				<< device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() 
				<< std::endl;
			std::cout
				<< "\t\tDevice Max Allocateable Memory: " 
				<< device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() 
				<< std::endl;
			std::cout 
				<< "\t\tDevice Local Memory: " 
				<< device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() 
				<< std::endl;
			std::cout 
				<< "\t\tDevice Type: " 
				<< device.getInfo<CL_DEVICE_TYPE>()
				<< std::endl;  
		}
		
	}
}

char* CV_OpenCL::read_binary_file(
	const std::string &xclbin_file_name, unsigned &nb) 
{
    std::cout << "INFO: Reading " << xclbin_file_name << std::endl;

	if(access(xclbin_file_name.c_str(), R_OK) != 0) {
		printf("ERROR: %s xclbin not available please build\n", xclbin_file_name.c_str());
		exit(EXIT_FAILURE);
	}
    //Loading XCL Bin into char buffer 
    std::cout << "Loading: '" << xclbin_file_name.c_str() << "'\n";
    std::ifstream bin_file(xclbin_file_name.c_str(), std::ifstream::binary);
    bin_file.seekg (0, bin_file.end);
    nb = bin_file.tellg();
    bin_file.seekg (0, bin_file.beg);
    char *buf = new char [nb];
    bin_file.read(buf, nb);
    return buf;
}