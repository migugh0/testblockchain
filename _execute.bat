::Copyright (c) 2016-2023 Knuth Project developers.
::Distributed under the MIT software license, see the accompanying
::file LICENSE or http://www.opensource.org/licenses/mit-license.php.
::SPDX-License-Identifier: MIT
@echo off
chcp 65001>nul
cd /d %~dp0
::ESP:Script para probar el ejecutable
echo %~nx0: Executing...
echo =======================
::
.\build\bin\testblockchain.exe
::
echo =======================
pause