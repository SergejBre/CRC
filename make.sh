#!/bin/bash

#===============================================================================
#  Home Office
#  Nürnberg, Germany
#  E-Mail: sergej1@email.ua
#
#  Copyright (C) 2021 free Project CRC Generic library. All rights reserved.
#------------------------------------------------------------------------------
#  The CRC Generic library is designed as a universal solution for most practical
#  CRC methods and provides a flexible API for communication application developers
#===============================================================================
#//! \file          make.sh
#//! \b Compiler:   script \n
#//! \b Target:     bash \n
#//! \b Repository: CRC
#//!
#//! This script compiles and builds CRC-Generic library and Tests.
#===============================================================================

################################################################################
# Functions
################################################################################

#//! This will trap any errors or commands with non-zero exit status
#//! by calling function catch_errors()
trap catch_errors ERR;

function catch_errors() {
	EXIT_STATUS=$?
    echo "======================= Script aborted because of errors. Please try to fix and rerun script. ========================"
	
	exit $EXIT_STATUS
}


################################################################################
# Get Script Execution Directory
################################################################################
SCRIPT_SOURCE="${BASH_SOURCE[0]}"
while [ -h "${SCRIPT_SOURCE}" ]; do
  SCRIPT_DIR="$(cd -P "$(dirname "${SCRIPT_SOURCE}")" && pwd)"
  SCRIPT_SOURCE="$(readlink "${SCRIPT_SOURCE}")"
  [[ "${SCRIPT_SOURCE}" != "/*" ]] && SCRIPT_SOURCE="${SCRIPT_DIR}/${SCRIPT_SOURCE}"
done
SCRIPT_DIR="$(cd -P "$(dirname "${SCRIPT_SOURCE}")" && pwd)"
echo "${SCRIPT_DIR}"


################################################################################
# Welcome message
################################################################################

echo "This script compiles and builds CRC-Generic library, Examples and Tests."
echo ""
echo "Prerequisites: 1) GNU compiler gcc (version >= 4.7.2) (gcc --version)"
echo "                  Compiled CRC-Generic library, Examples and Tests."
echo "               2) Selection of the macro -D OPTIMIZE for:"
echo "                  performance =BUILD_FOR_PERFORM"
echo "                  code size =BUILD_FOR_SIZE"
echo "                  data size =BUILD_FOR_DATA"
echo ""


################################################################################
# Some input checks
################################################################################

if [ $# != 0 ]
then 
	echo "This script must be run without any parameters"
	echo ""
	exit 1
fi

if [[ ! -e make.sh ]]
then
	echo "This script must be run from the root directory of the CRC-Generic project (missing make.sh)"
	echo ""
	exit 2
fi

#if [[ ! -e doc/html ]]
#then
#	echo "No doxygen documentation found"
#	echo ""
#	exit 3
#fi

#if [[ ! -e src/api_crc.c || ! -e tst/tests.c ]]
#then
#	echo "No source files found"
#	echo ""
#	exit 4
#fi


################################################################################
# Compile and build CRC-Generic library and tests
################################################################################

#gcc FLAGS for Optimization of Code Size
#CFLAGS="-pipe -Os -Wall -W -fPIE -DOPTIMIZE=BUILD_FOR_SIZE"

#gcc FLAGS for Optimization of Data
#CFLAGS="-pipe -O2 -Wall -W -fPIE -DOPTIMIZE=BUILD_FOR_DATA"

#gss FLAGS for Optimization of Performance
CFLAGS="-pipe -O2 -Wall -W -fPIE -DOPTIMIZE=BUILD_FOR_PERFORM"

#gcc FLAGS for Profiling gprof
#CFLAGS="-pg -funroll-loops -fmodulo-sched -O2 -Wall -W"
#CFLAGS="-pg -pipe -Os -Wall -W -fPIE"

#gcc FLAGS for CRC-Process
PFLAGS="-pipe -funroll-loops -O3 -Wall -W -fPIE"
#gcc FLAGS for CRC-Process with profiler
#PFLAGS="-pg -pipe -funroll-loops -O3 -Wall -W -fPIE"

echo  "gcc FLAGS:" $CFLAGS
echo  "gcc PFLAGS for crc-process:" $PFLAGS

#build static CRC lib
echo  "Compile and build CRC-Generic static library"
echo  "build libCRC_generic_static.a .."
echo  ""
cd    lib
if [[ -e libCRC_generic_static.a ]]
	then
	rm libCRC_generic_static.a
fi
gcc   -c $CFLAGS ../src/api_crc.c

case $CFLAGS in
	*BUILD_FOR_PERFORM*)
     echo BUILD_FOR_PERFORM
	 gcc   -c $PFLAGS ../src/api_crc_process.c
	 ar    rcs libCRC_generic_static.a api_crc.o api_crc_process.o
	 ;;
    *BUILD_FOR_SIZE*)
     echo BUILD_FOR_SIZE
	 ar    rcs libCRC_generic_static.a api_crc.o
	 ;;
    *BUILD_FOR_DATA*)
     echo BUILD_FOR_DATA
	 ar    rcs libCRC_generic_static.a api_crc.o
     ;;
esac
LINK_LIB_STAT="-L../lib -lCRC_generic_static"

#build dynamic CRC lib
echo  "Compile and build CRC-Generic dynamic library"
echo  "build libCRC_generic_dynamic.so .."
echo  ""
gcc   -c $CFLAGS -fPIC ../src/api_crc.c

case $CFLAGS in
	*BUILD_FOR_PERFORM*)
	 gcc   -c $PFLAGS -fPIC ../src/api_crc_process.c
	 gcc   -shared -Wl,-soname,libCRC_generic_dynamic.so.1 -o libCRC_generic_dynamic.so.1.0.1 api_crc.o api_crc_process.o
	 ;;
    *BUILD_FOR_SIZE*)
	 gcc   -shared -Wl,-soname,libCRC_generic_dynamic.so.1 -o libCRC_generic_dynamic.so.1.0.1 api_crc.o
	 ;;
    *BUILD_FOR_DATA*)
	 gcc   -shared -Wl,-soname,libCRC_generic_dynamic.so.1 -o libCRC_generic_dynamic.so.1.0.1 api_crc.o
     ;;
esac
ln    -s -f libCRC_generic_dynamic.so.1.0.1 libCRC_generic_dynamic.so.1
ln    -s -f libCRC_generic_dynamic.so.1.0.1 libCRC_generic_dynamic.so
rm    -f api_crc.o api_crc_process.o
LINK_LIB_DIN="-L../lib -lCRC_generic_dynamic -Wl,-rpath,../lib"

cd    ..

exit 0

