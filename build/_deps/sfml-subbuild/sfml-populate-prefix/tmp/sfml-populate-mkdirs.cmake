# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Projects/sfml-imgui-engine/build/_deps/sfml-src"
  "D:/Projects/sfml-imgui-engine/build/_deps/sfml-build"
  "D:/Projects/sfml-imgui-engine/build/_deps/sfml-subbuild/sfml-populate-prefix"
  "D:/Projects/sfml-imgui-engine/build/_deps/sfml-subbuild/sfml-populate-prefix/tmp"
  "D:/Projects/sfml-imgui-engine/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
  "D:/Projects/sfml-imgui-engine/build/_deps/sfml-subbuild/sfml-populate-prefix/src"
  "D:/Projects/sfml-imgui-engine/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Projects/sfml-imgui-engine/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Projects/sfml-imgui-engine/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
