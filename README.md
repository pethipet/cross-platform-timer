# A Cross-platform-timer

## CI
[![CMake](https://github.com/pethipet/cross-platform-timer/actions/workflows/cmake.yml/badge.svg)](https://github.com/pethipet/cross-platform-timer/actions/workflows/cmake.yml)

## Overview
Small example to create low resolution timer handling in C for Windows and POSIX/Linux System

## Feature
* Abstract timer API - hiding implementation
* Platform support POSIX / Linux and Windows 

* Testbet to learn current build systems
** CMake
** Meson
** github action/runner  


## Getting started

This project is hosted on GitHub. 
You can clone the project directly using this command:

> git clone --recursive https://github.com/pethipet/cross-platform-timer.git


## How to build?
This projects build with CMake and Meson.

## CMake
> \$ cd cross-platform-time
>
> \$ cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
> 
> ...
>
> \$ cmake --build build --config RelWithDebInfo
>
> \$ cd build/bin/RelWithDebInfo
> 
> HERE you find some example binaries build


## Meson

> \$ cd cross-platform-timer
>
> \$ meson setup builddir
>
> \$ cd builddir
>
> \$ meson compile
>
> \$ meson test

