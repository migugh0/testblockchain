::Copyright (c) 2016-2023 Knuth Project developers.
::Distributed under the MIT software license, see the accompanying
::file LICENSE or http://www.opensource.org/licenses/mit-license.php.
::SPDX-License-Identifier: MIT
@echo off
chcp 65001>nul
cd /d %~dp0
::ESP:Script para la instalación previa en Windows
IF NOT EXIST install cd ..
echo =========================
echo ==Instalamos en Windows==
echo =========================
::Información inicial
echo Recuerda usar un entorno compatible con C++20
echo.
echo CMake habilita automáticamente los modos de depuración CMake Server o cmake-file-api para las versiones 3.7.2 y posteriores.
echo La depuración de CMake no está disponible en versiones anteriores.
echo.
echo Conan adopta el enfoque de integración de herramientas que ofrece CMake y no admite llamadas desde CMake.
echo Esto significa que deberá llamar a conan install antes de comenzar a trabajar.
echo.
::
echo Requerido: CMake^>^=3.23
where cmake 1>nul 2>&1
IF NOT %ERRORLEVEL% == 0 (
	echo ERROR cmake command no encontrado/no instalado
	echo Instalación no realizada
	exit
)
echo Actual CMake:
cmake --version | findstr /r [0-2]
::
echo Requerido: Conan^>^=1.51
where conan 1>nul 2>&1
IF NOT %ERRORLEVEL% == 0 (
	echo ERROR conan command no encontrado/no instalado
	echo Instalación no realizada
	exit
)
echo Actual Conan:
conan --version | findstr /r [0-2]
echo Para comenzar a configurar conan en el directorio actual pulsa una tecla...
REM pause>nul
::
IF NOT DEFINED CONAN_USER_HOME (
	echo CONAN_USER_HOME no definido:
	setx CONAN_USER_HOME %USERPROFILE%
	REM export CONAN_USER_HOME /c/home/Usuario
	echo Instalación de Conan establecida en %USERPROFILE%
	echo Por favor reinicia la consola para recargar la variable CONAN_USER_HOME
	pause>nul
	exit
)
::
echo ==Limpiamos instalación de usuario y dejamos los profiles por defecto:==
echo ========================================================================
REM echo Realizamos reset a la carpeta .conan establecida en CONAN_USER_HOME
REM conan remove -f '*'
REM echo Borramos paquetes antiguos que tienen nuevas versiones más actualizadas
REM conan remove "*" -s -b -o -f
REM echo Instalamos la configuración inicial en %CONAN_USER_HOME%/.conan
REM conan config init --force
REM echo Activamos libstdc++11 en los profiles
REM conan profile update settings.compiler.libcxx=libstdc++11 default
IF NOT EXIST %CONAN_USER_HOME%/.conan (
	conan config install -t dir %CONAN_USER_HOME%/.conan
) ELSE (
    echo .conan ya existe en %CONAN_USER_HOME%, no hace falta instalar los profiles por defecto
)
::
echo ==Instalamos profiles locales:==
echo ================================
IF NOT EXIST .conan (
	echo Ten en cuenta que se cargarán los profiles locales, la configuración local del repositorio primero
	echo y si no existe usara la configuración del usuario.
	conan config install -t dir .conan
) ELSE (
    echo .conan ya existe en el repositorio, no hace falta instalar los profiles locales
)
::
echo ==Creamos los archivos de configuraciones si no existen:==
echo ==========================================================
IF NOT EXIST conanfile.py (
    echo conanfile.py no existe
	echo Configuramos conanfile.py para usar el perfil ninja
	conan lock create conanfile.py --version=0.0.0 -pr:b=ninja -pr:h=ninja --lockfile=conan.lock --lockfile-out=build/conan.lock
)
mkdir build 1>nul 2>&1
echo borramos build/conan.lock y la regeneramos de nuevo
del build\conan.lock 1>nul 2>&1
IF EXIST build\conan.lock echo %0: The script cannot remove "build/conan.lock" file, please use root to remove it
echo
conan install conanfile.py --lockfile=build/conan.lock -if build --build missing
::
echo ======================
echo Instalación completada
exit


::Orden correcto original de comandos sueltos POSIX::
::En WINDOWS con batch usar: setx CONAN_USER_HOME %USERPROFILE%
::En POSIX con batch usar: setx CONAN_USER_HOME C:\msys64\home\Usuario
::En POSIX con bash usar: export CONAN_USER_HOME /c/home/Usuario
gcc --version
g++ --version
cmake --version
conan --version
echo usa este comando o borra la carpeta .conan
conan remove -f '*'
echo Configuramos los perfiles minimos en %CONAN_USER_HOME%
echo Usa para ello estos 2 comandos para configurar la estructura mínima necesaria para el perfil por defecto
echo Sin embargo es mucho más completo y rápido usar conan install (que te autodetecta tu configuración)
conan config init --force
conan profile update settings.compiler.libcxx=libstdc++11 default
echo Coloca aquí la configuración de tus perfiles, aquí dejo para el de UCRT64 
conan profile update settings.os=Windows default
conan profile update settings.os_build=Windows default
conan profile update settings.arch=x86_64 default
conan profile update settings.arch_build=x86_64 default
conan profile update settings.build_type=Release default
conan profile update settings.compiler=gcc default
conan profile update settings.compiler.version=11 default
conan profile update env.CC=C:\msys64\ucrt64\bin\gcc.exe default
conan profile update env.CXX=C:\msys64\ucrt64\bin\g++.exe default
conan profile update conf.tools.cmake.cmaketoolchain:generator=Ninja ninja
echo Accedemos a %CONAN_USER_HOME% o bien pon aquí el *directorio donde esta descargado el repositorio*
cd %CONAN_USER_HOME%
conan config install -t dir .conan
echo Configuramos conanfile.py para usar el perfil local ninja
conan lock create conanfile.py --version=0.0.0 -pr:b=ninja -pr:h=ninja --lockfile=conan.lock --lockfile-out=build/conan.lock
echo Regeneramos el conan.lock dentro de la carpeta build para ello lo borramos previamente ya que si no Conan no lo sustituye
conan install conanfile.py --lockfile=build/conan.lock -if build --build missing
