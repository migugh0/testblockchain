::Copyright (c) 2016-2023 Knuth Project developers.
::Distributed under the MIT software license, see the accompanying
::file LICENSE or http://www.opensource.org/licenses/mit-license.php.
::SPDX-License-Identifier: MIT
@echo off
chcp 65001>nul
cd /d %~dp0
::ESP:Script para compilar y enlazar
echo %~nx0: Compiling and linking...
echo =======================
::
echo building .exe
::make --quiet -C build/out -f Makefile -j 1
ninja --quiet -C build/out -f build.ninja -k 1
::
echo =======================
::Pause
IF %ERRORLEVEL% == 0 (
	echo %~nx0: Compilation and linking successfull
) ELSE (
	echo %~nx0: Check Errors!!!
)
echo =======================