# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/zlib-src"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/zlib-build"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/zlib-subbuild/zlib-populate-prefix"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/zlib-subbuild/zlib-populate-prefix/tmp"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/zlib-subbuild/zlib-populate-prefix/src/zlib-populate-stamp"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/zlib-subbuild/zlib-populate-prefix/src"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/zlib-subbuild/zlib-populate-prefix/src/zlib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/zlib-subbuild/zlib-populate-prefix/src/zlib-populate-stamp/${subDir}")
endforeach()
