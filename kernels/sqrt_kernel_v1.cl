__kernel void compute_sqrt(__global float* x,
                           int array_size){
    for (int i = 0; i < array_size; i++)
    {
      x[i] = sqrt(x[i]);
    }
}
