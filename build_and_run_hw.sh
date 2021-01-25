#!/bin/bash

set -e

HW_TARGET=hw make all
XCL_EMULATION_MODE=hw ./sqrt 100 v1 hw