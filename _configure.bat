::Copyright (c) 2016-2023 Knuth Project developers.
::Distributed under the MIT software license, see the accompanying
::file LICENSE or http://www.opensource.org/licenses/mit-license.php.
::SPDX-License-Identifier: MIT
@echo off
chcp 65001>nul
cd /d %~dp0
::ESP:Script para configurar cmake antes de compilar
echo %~nx0: Configuring cmake...
echo =============
::
mkdir build 1>nul 2>&1
cd build
rmdir /s /q out 1>nul 2>&1
IF EXIST out echo _configure.bat: Script cannot remove "out" folder, please use root to remove it
mkdir out 1>nul 2>&1
mkdir bin 1>nul 2>&1
cd ..
::
::cmake -S <path-to-source> -B <path-to-build> -G "<Generator>" [options]
::cmake -S . -B build/out -G "Unix Makefiles" --log-level=ERROR
::cmake -S . -B build/out -G "MSYS Makefiles" --log-level=ERROR
::cmake -S . -B build/out -G "MinGW Makefiles" --log-level=ERROR
cmake -S . -B build/out -G "Ninja" --log-level=ERROR
::
::If no errors remember to compile after too
IF %ERRORLEVEL% == 0 (
	echo =============
	echo %~nx0: No errors encountered after configuring cmake
)