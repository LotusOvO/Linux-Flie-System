cmake_minimum_required(VERSION 3.16)
project(os)

set(CMAKE_CXX_STANDARD 20)

add_executable(os main.cpp Format.h header.h blockinode.h ls.h md.h cd.h rd.h mf.h cat.h del.h copy.h info.h check.h)
