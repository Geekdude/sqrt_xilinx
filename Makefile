HW_TARGET ?= sw_emu
VERSION ?= 1

#HWC stands for hardware compiler
HWC = v++
TMP_DIR = _x/$(HW_TARGET)/$(VERSION)
src_files = main_xilinx.cpp cv_opencl.cpp sqrt.cpp
hpp_files = cv_opencl.hpp sqrt.hpp
KERNEL_SRC = kernels/sqrt_kernel_v$(VERSION).cl
COMPUTE_SQRT_XO = $(HW_TARGET)/xo/compute_sqrt_v$(VERSION).xo
XCLBIN_FILE = $(HW_TARGET)/sqrt_kernel_v$(VERSION).xclbin

.PHONY: all kernel
all: sqrt emconfig.json $(XCLBIN_FILE)
build: $(COMPUTE_SQRT_XO)
kernel: $(XCLBIN_FILE)

sqrt: $(src_files) $(hpp_files)
	g++ -Wall -g -std=c++11 $(src_files) -o $@ -I../common_xlx/ \
	-I${XILINX_XRD}/include/ -L${XILINX_XRT}/lib/ -L../common_xlx -lOpenCL \
	-lpthread -lrt -lstdc++

emconfig.json:
	emconfigutil --platform xilinx_u250_xdma_201830_2 --nd 1

$(COMPUTE_SQRT_XO): $(KERNEL_SRC)
	$(HWC) -c -t $(HW_TARGET) --kernel compute_sqrt --temp_dir $(TMP_DIR) \
	--config design.cfg -Ikernels -I. $< -o $@

$(XCLBIN_FILE): $(COMPUTE_SQRT_XO)
	$(HWC) -l -t $(HW_TARGET) --temp_dir $(TMP_DIR) --config design.cfg \
	--connectivity.nk=compute_sqrt:1:csq_1 \
	$^ -I. -o $@

.PHONY: clean
clean:
	rm -rf sqrt emconfig.json xo/ built/ sw_emu/ hw_emu/ hw/ _x *.log .Xil/
