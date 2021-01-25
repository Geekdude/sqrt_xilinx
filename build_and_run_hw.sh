#!/bin/bash

set -e

HW_TARGET=hw make all
./sqrt 100 v1 hw