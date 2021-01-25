#!/bin/bash

set -e

make all
XCL_EMULATION_MODE=sw_emu ./sqrt 100 v1 sw_emu