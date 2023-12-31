# Sipsyphus_compare

## Description
This project aims to compare two RPM repositories and analyze the differences between them. It utilizes the following libraries: curllib, rpmlib, rapidjson.

## Features
- Fetches RPM metadata from two repositories using curllib.
- Parses and processes RPM metadata using rpmlib.
- Compares the RPM packages between the two repositories.
- Provides detailed analysis of the differences found.
- Uses rapidjson for efficient JSON parsing and manipulation.

## Installation
1. Clone the repository:
   
   git clone https://github.com/aiburegit/Sipsyphus_compare.git
   
2. Install the required dependencies:
   
   sudo apt-get install libcurl-devel
   sudo apt-get install librpm-devel
   sudo apt-get install rapidjson
   
3. Build the project:
   !default path(PREFIX) is /usr/local using sudo
   1. cd project
   2. make PREFIX=(path) install
   3. LD_LIBRARY_PATH=(path)/lib
   you will also have to specify the path for LD with the following command: LD_LIBRARY_PATH=(path)/lib - it is a location a shared lib

   #Install to the folder where the project itself is located - make PREFIX=. install
   #LD_LIBRARY_PATH=./lib

## Usage
1. Run the executable with the following command:
   
   ./repcmp <repository1_url> <repository2_url>
   ./repcmp sisyphus p10
   
2. Replace <repository1_url> and <repository2_url> with the URLs of the RPM repositories you want to compare.
3. The program will fetch metadata from both repositories, compare the packages, and provide a detailed analysis of the differences found.
4. You can use the "-u" switch to specify the full address

   ./repcmp -u https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus https://rdb.altlinux.org/api/export/branch_binary_packages/p10


## Dependencies
- curllib: A library for making HTTP requests and handling responses.
- rpmlib: A library for parsing and processing RPM metadata.
- rapidjson: A fast JSON parser and generator for C++.
