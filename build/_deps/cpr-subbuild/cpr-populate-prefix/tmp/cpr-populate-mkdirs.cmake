# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/cpr-src"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/cpr-build"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/cpr-subbuild/cpr-populate-prefix"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/cpr-subbuild/cpr-populate-prefix/tmp"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/cpr-subbuild/cpr-populate-prefix/src/cpr-populate-stamp"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/cpr-subbuild/cpr-populate-prefix/src"
  "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/cpr-subbuild/cpr-populate-prefix/src/cpr-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/aibure/CppProjects/Sisyphus_compare/build/_deps/cpr-subbuild/cpr-populate-prefix/src/cpr-populate-stamp/${subDir}")
endforeach()
