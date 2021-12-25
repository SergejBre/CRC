# CRC Generic library
<p align="center">
	<a href="https://gcc.gnu.org/" rel="nofollow">
		<img alt="Platform GNU/GCC" src="https://img.shields.io/badge/platform-GNU/GCC-blue.svg"/>
	</a>
	<a href="https://osdn.net/projects/mingw/" rel="nofollow">
		<img alt="Platform MinGW" src="https://img.shields.io/badge/platform-MinGW-green.svg"/>
	</a>
	<a href="https://github.com/SergejBre/CRC/releases">
		<img alt="releases page" src="https://img.shields.io/github/v/release/SergejBre/CRC.svg"/>
	</a>
	<a href="https://github.com/SergejBre/CRC/releases">
		<img alt="Github All Releases" src="https://img.shields.io/github/downloads/SergejBre/CRC/total.svg"/>
	</a>
	<a href="https://github.com/SergejBre/CRC/issues">
		<img alt="Issues" src="https://img.shields.io/github/issues/SergejBre/CRC.svg"/>
	</a>
	<a href="https://sergejbre.github.io/CRC/doc/html/index.html">
		<img alt="Docs Doxygen" src="https://img.shields.io/badge/docs-Doxygen-blue.svg"/>
	</a>
	<a href="https://github.com/SergejBre/CRC/blob/main/LICENSE">
		<img alt="License" src="https://img.shields.io/github/license/SergejBre/CRC.svg"/>
	</a>
</p>

## Contents
* [Specification](#Specification)
* [Installation instructions](#Installation-instructions)
* [Directory structure](#Directory-structure)
* [CRC Generic Library Interface](#CRC-Generic-Library-Interface)
* [CRC Generic process workflow](#CRC-Generic-process-workflow)
* [The Basics of CRC calculation of the residual value and Examples](#The-Basics-of-CRC-calculation-of-the-residual-value-and-Examples)
  * [LFSR Algorithm CRC Algorithm with the linear feedback shift register](#LFSR-Algorithm-CRC-Algorithm-with-the-linear-feedback-shift-register)
  * [LUT Algorithm CRC Algorithm with the lookup table](#LUT-Algorithm-CRC-Algorithm-with-the-lookup-table)
  * [CRC Generic Algorithm api_crc_process](#CRC-Generic-Algorithm-api_crc_process)
* [Build process and integration of the CRC Generic library in applications](#Build-process-and-integration-of-the-CRC-Generic-library-in-applications)
  * [Optimization and Performance of the CRC Process](#Optimization-and-Performance-of-the-CRC-Process)
  * [The test concept](#The-test-concept)
  * [Run tests](#Run-tests)
* [Documentation](#Documentation)
* [Tutorium for CRC Generic library](#Tutorium-for-CRC-Generic-library)
* [License](#License)

## Specification
The cyclic redundancy check (CRC) is a method for determining a check value for data in order to detect errors in transmission or storage binary data. Ideally, the process can even correct the received data automatically to avoid retransmission. The CRC Generic library is designed as a universal solution for most practical CRC methods and provides a flexible API for communication application developers.

The following features are offered of the CRC library (see also part [CRC Generic process workflow](#CRC-Generic-process-workflow):
* A interface for setting CRC process and the initiation of relevant data
* Check and validation of the initial data
* The use of all relevant generators polynomials up to and including *CRC-64*
* Works with any data formats (MSB, and LSB order of the significant bits)
* The library processed the data streams with a offset .

The library is currently available as:
* 64-bit Linux static library (*lib/libCRC_generic_static.a*)
* 64-bit Linux shared library (*lib/libCRC_generic_dynamic.so*) .

The library is created and can be used with the GNU GCC compiler (version 4.7 or newer) under Linux or with the MinWG GNU compiler (version 4.8 or newer) under Windows. The installation process is described [below](#Installation-instructions).

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## Installation instructions
The library is built and can be used with GNU compiler gcc (version 4.7.2 or newer) and 64-bit GNU/Linux.

For the installation user privileges are sufficient, except if additional linux packages have to be installed. In the following, we denote user privileges with a dollar sign ('$') and root privileges with a hash sign ('#').

Based on a clean Linux 64-bit with standard system utilities please install:
```
# apt-get install build-essential
```
Check version of gcc:
```
$ gcc -v
...
gcc version 4.7.2 (Debian 4.7.2-5)
```
Extract CRC-Generic library, which is delivered as a compressed tar archive. It is assumed that the CRC-Generic library is extracted in the user's home directory with the path name as shown below. This path name is used for the rest of this document, but a different path name can be used without restrictions.
```
~$ tar -xvf CRCgeneric_release_YYYY-MM-DD.tar.xz -C ~/
~$ cd crc
~/crc$ ls
cfg doc fig inc lib LICENSE  make.sh  README.md src tst tut
```
See also [Directory structure](#Directory-structure).

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## Directory structure
The following directories and files are part of the CRC-Generic library:
```
~/crc
    |
    +--/.git (git repository)
    |
    +--/cfg (Configuration)
    |     |
    |     +--api_crc_cfg.h (Configuration head file for the CRC-Generic library)
    |
    +--/doc (Documentation)
    |     |
    |     +--Projekt CRC-Generic.pdf (Grundlagen und Beschreibung vom CRC-Prozess)
    |     |
    |     +--Tutorium for CRC.pdf (Tutorial for the CRC-Generic library)
    |
    +--/fig (Figures to CRC-Generic project)
    |     |
    |     +--CRC-Generic_algorithmLFSR.png (algorithm LFSR picture)
    |     |
    |     +--CRC-Generic_CRCprocess.png (CRC-Process picture)
    |     |
    |     +--CRC-Generic_LUTprocess.png (LUT-Process picture)
    |     |
    |     +--CRC-Generic_processOverview.png (Overview process picture)
    |     |
    |     +--Direct Forward LUT Algorithm.png
    |     |
    |     +--Direct Straightforward SR Algorithm.png
    |     |
    |     +--Division-Register.png
    |     |
    |     +--Reflected Table-Driven.png
    |     |
    |     +--Reversed LUT Algorithm.png
    |     |
    |     +--Reversed Straightforward SR Algorithm.png
    |     |
    |     +--Table-Driven.png
    |
    +--/inc (Header files for the CRC-Generic library)
    |     |
    |     +--api_crc.h (head file for the CRC-Generic library)
    |     |
    |     +--api_crc_process.h (head file for the CRC-Generic library. Process functions)
    |
    +--/lib (directory for object files and compilation results (static and dynamic library)
    |     |
    |     +--libCRC_generic_dynamic.so (symbolic link for dynamic CRC-Generic library)
    |     |
    |     +--libCRC_generic_dynamic.so.1 (symbolic link for dynamic CRC-Generic library)
    |     |
    |     +--libCRC_generic_dynamic.so.1.x.y (dynamic CRC-Generic library)
    |     |
    |     +--libCRC_generic_static.a (static CRC-Generic library)
    |
    +--/src (Source files for the CRC-Generic library)
    |     |
    |     +--api_crc.c (source file for the CRC-Generic library)
    |     |
    |     +--api_crc_process.c (source file for the CRC-Generic library. Process functions)
    |
    +--/tst (Tests to the CRC-Generic project, directory for tests files)
    |     |
    |     +--tests.c (source file for Unit-Test)
    |
    +--/tut (Tutorial to the CRC-Generic project)
    |     |
    |     +--examples.h (head file for Examples)
    |     |
    |     +--examples.c (source file for Examples)
    |     |
    |     +--examples*_in.bin (input data file for Examples*)
    |     |
    |     +--examples*_out.bin (output data file for Examples*)
    |
    +--LICENSE
    |
    +--make.sh (make file for the CRC-Generic library)
    |
    +--README.md
```

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## CRC Generic Library Interface
The Interface of the CRC Generic library consists of the 6 public functions:
* The initialization function: *api_crc_init*
* The LUT table function: *api_crc_table*
* The function gives size of the table back: *api_crc_sizeofTable*
* The method for cleaning of old data: *api_crc_reset*
* The method for calculate the CRC proof sum: *api_crc_process*
* The method for return the requested CRC value: *api_crc_finalize*
```c
// The initialization function is used to fill the data necessary for the calculation CRC.
api_crc_status_e api_crc_init(uint64_t const		polynomial,
                               uint64_t const		inputXOR,
                               uint64_t const		outputXOR,
                               uint64_t const *const	table,
                               api_crc_order_e const	inputOrder,
                               api_crc_order_e const	outputOrder,
                               api_crc_device_s *const 	device
                               );

// The function fills the LUT table with the previously calculated values.
api_crc_status_e api_crc_table(api_crc_device_s * 	device,
                               uint64_t *const		table,
                               uint16_t 		size
                               );

// This function returns the size of LUT table.
uint16_t api_crc_sizeofTable(uint64_t const 	polynomial);

// The method used to api_crc_reset cleaning of old data.
api_crc_status_e api_crc_reset(api_crc_device_s const *const 	device,
                               api_crc_state_t *const		state
                               );

// The method calculates the CRC proof sum.
api_crc_status_e api_crc_process(api_crc_device_s const *const	device,
                                 api_crc_state_t *const 	state,
                                 uint8_t const *const		buffer,
                                 uint8_t const			offset,
                                 uint32_t const			size
                                 );		

// This method returns the requested value and taking order predetermined values outputOrder,
// outputOrder and the value outputXOR.
api_crc_status_e api_crc_finalize(api_crc_device_s const *const	device,
                                  api_crc_state_t *const 	state 
                                  );
```
a data structure: *api_crc_device_s*
```c
// Device structure for calculating a Cyclic Redundancy Code (CRC) checksum.
typedef struct
{
   // Generator polynomial for calculating the CRC checksum.
   uint64_t polynomial;
   // Initial value for XORing with the polynomial division state register.
   uint64_t inputXOR;
   // Final value for XORing with the polynomial division state register.
   uint64_t outputXOR;
   // Lookup table for processing one input byte per iteration.
   uint64_t const *table;
   // Bit order for processing input bytes.
   api_crc_order_e inputOrder;
   // Bit order for storing the CRC checksum.
   api_crc_order_e outputOrder;
} api_crc_device_s;
```
and a state variable: *api_crc_state_t*
```c
// State register for storing intermediate as well as the final value of the Cyclic Redundancy Code (CRC)
// checksum calculation.
typedef uint64_t api_crc_state_t;
```
The detailed description of the methods is in the documentation section API_LIBRARY_CRC_INTERFACE

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## CRC Generic process workflow
TODO

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## The Basics of CRC calculation of the residual value and Examples
### LFSR Algorithm CRC Algorithm with the linear feedback shift register
TODO
### LUT Algorithm CRC Algorithm with the lookup table
TODO
### CRC Generic Algorithm api_crc_process
TODO

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## Build process and integration of the CRC Generic library in applications
### Optimization and Performance of the CRC Process
TODO
### The test concept
TODO
### Run tests
TODO

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## Documentation
TODO

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## Tutorium for CRC Generic library
TODO

[![Top](https://img.shields.io/badge/back%20to%20top-%E2%86%A9-blue)](#Contents)
____
## License
[![MIT](https://img.shields.io/github/license/SergejBre/SynchroTime.svg)](LICENSE)
