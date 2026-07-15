@echo off
setlocal
REM Resolve zig
if defined ZIG_BIN (
  if exist "%ZIG_BIN%" set "ZIG=%ZIG_BIN%" & goto zig_found
)
where zig >nul 2>nul
if %ERRORLEVEL%==0 (
  for /f "delims=" %%i in ('where zig') do set "ZIG=%%i" & goto zig_found
)
if exist "%USERPROFILE%\.local\bin\zig.exe" set "ZIG=%USERPROFILE%\.local\bin\zig.exe" & goto zig_found
if exist "%USERPROFILE%\bin\zig.exe" set "ZIG=%USERPROFILE%\bin\zig.exe" & goto zig_found
echo zig not found (tried ZIG_BIN, PATH, %%USERPROFILE%%\.local\bin\zig.exe)>&2
exit /b 1
:zig_found
REM Resolve python
if defined PYTHON_BIN (
  if exist "%PYTHON_BIN%" set "PY=%PYTHON_BIN%" & goto py_found
)
where python >nul 2>nul
if %ERRORLEVEL%==0 (
  for /f "delims=" %%i in ('where python') do set "PY=%%i" & goto py_found
)
where python3 >nul 2>nul
if %ERRORLEVEL%==0 (
  for /f "delims=" %%i in ('where python3') do set "PY=%%i" & goto py_found
)
echo python not found>&2
exit /b 1
:py_found
echo zig: 
call "%ZIG%" version
echo python:
call "%PY%" --version
call "%ZIG%" cc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror parse_lab.c -o parse_lab_check.exe
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
del parse_lab_check.exe 2>nul
del parse_lab.exe 2>nul
call "%PY%" -m py_compile run_lab.py test_lab.py
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
call "%PY%" run_lab.py
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
call "%PY%" -m unittest -v
exit /b %ERRORLEVEL%
