/********************************************************************
	SQRT.hpp
********************************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "cv_opencl.hpp"

#define AOCL_ALIGNMENT 64

inline static void* alignedMalloc(size_t size)
{
	void *ptr = NULL;
	if ( posix_memalign (&ptr, AOCL_ALIGNMENT, size) )
	{
		fprintf(stderr, "Aligned Malloc failed due to insufficient memory.\n");
		exit(-1);
	}
	return ptr;
}

template <typename T>
cl::Buffer alloc(const CV_OpenCL& cv, int n, T* data){
  cl_int err;
  cl::Buffer buf;
  OCL_CHECK(err, buf = cl::Buffer(cv.ctx,
                                  CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                  sizeof(T) * n,
                                  data,
                                  &err));

  if (err != 0) {
    exit(1);
  }
  return buf;
}

typedef struct SQRT
{
  CV_OpenCL& cv_opencl;

  cl::Buffer x_device;
  float* x_host;
  int array_size;

  SQRT(CV_OpenCL& opencl, int array_size);

  void compute_sqrt();
} SQRT;