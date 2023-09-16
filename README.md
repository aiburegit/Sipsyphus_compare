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
   cd project
   make PREFIX=(path) install
   #Install to the folder where the project itself is located - make PREFIX=. install
   

## Usage
1. Run the executable with the following command:
   
   ./repcmp <repository1_url> <repository2_url>
   
2. Replace <repository1_url> and <repository2_url> with the URLs of the RPM repositories you want to compare.
3. The program will fetch metadata from both repositories, compare the packages, and provide a detailed analysis of the differences found.
4. You can use the "-u" switch to specify the full address



## Dependencies
- curllib: A library for making HTTP requests and handling responses.
- rpmlib: A library for parsing and processing RPM metadata.
- rapidjson: A fast JSON parser and generator for C++.
