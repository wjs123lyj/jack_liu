#!/bin/bash

echo –e "hostname\t `hostname`"

echo –e "OScore\t `uname -r`"

echo –e "CPUInfo\t `grep "model name" /proc/cpuinfo|awk –F: '{print $2}'`"

echo –e "CPUMHz\t `grep "MHZ" /proc/cpuinfo | awk –F "：" '{print $2}'`"

echo –e "MEMTotal\t `free | awk '$1="Mem:"{print $2}'`"

echo –e "DiskInfo\t `df | grep "dev" |awk '{print $1 $2}'`"
