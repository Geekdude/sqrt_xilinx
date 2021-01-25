#include <chrono>
#include "sqrt.hpp"

SQRT::SQRT(CV_OpenCL &opencl, int array_size) : cv_opencl(opencl)
{
    this->array_size = array_size;

    // Setup Buffers
    x_host = (float *)alignedMalloc(sizeof(float)*array_size);

    for (int i = 0; i < array_size; i++)
    {
        x_host[i] = 25;
    }

    x_device = alloc<float>(cv_opencl, array_size, x_host);
    cv_opencl.cmd_q.enqueueMigrateMemObjects({x_device}, 0);
    cv_opencl.cmd_q.finish();

    std::cout << "Starting..." << std::endl;

    // Start Time.
    std::chrono::steady_clock::time_point tick;
    tick = std::chrono::steady_clock::now();

    compute_sqrt();

    cv_opencl.cmd_q.finish();

    // Download results
    cv_opencl.cmd_q.enqueueMigrateMemObjects({x_device}, CL_MIGRATE_MEM_OBJECT_HOST);

    // Stop Time
    auto tock = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_delta = tock-tick;
    std::cout << "Computation Duration: " << time_delta.count() << " s" << std::endl;

    // Save duration to file.
    {
      std::ofstream file("duration.txt");
      file << "Computation Duration: " << time_delta.count() << " s" << std::endl;
    }

    // Save Results to file
    std::cout << "Saving solution..." << std::endl;
    {
      std::ofstream file("results.txt");
      for (int i = 0; i < array_size; i++)
      {
          file << x_host[i] << std::endl;
      }
    }
    std::cout << "Saved solution to results.txt..." << std::endl;

    std::cout << "Done..." << std::endl;
}

void SQRT::compute_sqrt()
{
    cl_int err;
    cl::Kernel kernel = cv_opencl.kernels[cv_opencl.compute_sqrt_idx];
    int arg_idx = 0;
    OCL_CHECK(err, err = kernel.setArg(arg_idx++, x_device));
    OCL_CHECK(err, err = kernel.setArg(arg_idx++, sizeof(int), &array_size));
    
    OCL_CHECK(err,
        err = cv_opencl.cmd_q.enqueueTask(
            kernel
        )
    );
}