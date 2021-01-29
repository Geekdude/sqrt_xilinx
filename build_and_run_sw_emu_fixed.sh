#!/bin/bash

set -e

# Full LD_LIBRARY_PATH from vitis shell.
# export LD_LIBRARY_PATH=/opt/Xilinx/Vitis/2020.1/lib/lnx64.o/Default:/opt/Xilinx/Vitis/2020.1/lib/lnx64.o:/opt/Xilinx/Vitis/2020.1/tps/lnx64/jre9.0.4/lib/:/opt/Xilinx/Vitis/2020.1/tps/lnx64/jre9.0.4/lib//server:/opt/Xilinx/Vitis/2020.1/lib/lnx64.o:/opt/Xilinx/Vitis/2020.1/lib/lnx64.o/Default:/opt/Xilinx/Vitis/2020.1/lib/lnx64.o:/opt/xilinx/xrt/lib::/opt/Xilinx/Vitis/2020.1/bin/../lnx64/tools/dot/lib

# Minimal LD_LIBRARY_PATH for simulation to load.
export LD_LIBRARY_PATH=/opt/Xilinx/Vitis/2020.1/lib/lnx64.o:/opt/xilinx/xrt/lib

make all
echo $LD_LIBRARY_PATH
XCL_EMULATION_MODE=sw_emu ./sqrt 100 v1 sw_emu
