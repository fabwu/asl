#!/bin/bash

modprobe msr
wrmsr -a 0x1a0 0x4000850089
