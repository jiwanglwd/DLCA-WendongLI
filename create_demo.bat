@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

REM Copyright (C) 2015 Zhang NS

REM This program is free software; you can redistribute it and/or modify
REM it under the terms of the GNU General Public License as published by
REM the Free Software Foundation; either version 2 of the License, or
REM (at your option) any later version.

REM This program is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM GNU General Public License for more details.

REM You should have received a copy of the GNU General Public License along
REM with this program; if not, write to the Free Software Foundation, Inc.,
REM 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

SET GIF_DELAY_D2=50
SET GIF_DELAY_D3=20
SET GIF_LOOP=1
SET GIF_FILENAME=animation

REM Frame per state
SET D3_FPS=5
SET D3_ELEV=30
SET D3_AZIM_0=0
REM Increment of AZIM per frame
SET D3_AZIM_FDELTA=1

IF "%1"=="" GOTO USAGE
IF "%2"=="" GOTO USAGE
IF "%3"=="" GOTO USAGE
IF "%4"=="" GOTO USAGE
IF "%5"=="" GOTO USAGE

SET D=%1
SET L=%2
SET N=%3
SET P=%4
SET M=%5

ECHO D = %D%
ECHO L = %L%
ECHO N = %N%
ECHO P = %P%
ECHO M = %M%

IF DEFINED ProgramFiles(x86) (
    SET WIN_VER=win64
) else (
    SET WIN_VER=win32
)

SET EXE=exe\%WIN_VER%\DLCA.exe

IF NOT EXIST %EXE% (
   ECHO %EXE% not found
   GOTO END
)

SET DEMO_DIR=demo\D%D%.L%L%.N%N%
IF NOT EXIST %DEMO_DIR% MKDIR %DEMO_DIR%

ECHO Running DLCA...
%EXE% %D% %L% %N% %DEMO_DIR%\D%D%.L%L%.N%N% %P%

ECHO Creating visualization...

IF %D%==2 (
    SET GIF_DELAY=%GIF_DELAY_D2%
    FOR %%f IN (%DEMO_DIR%\*.txt) DO (
        ECHO %%~nf.txt
        python plot_2d.py %L% %%f %DEMO_DIR%\%%~nf.png %M%
    )
) ELSE IF %D%==3 (
    SET GIF_DELAY=%GIF_DELAY_D3%
    SET AZIM=%D3_AZIM_0%
    FOR %%f IN (%DEMO_DIR%\*.txt) DO (
        ECHO %%~nf.txt
        FOR /L %%i IN (1, 1, %D3_FPS%) DO (
            SET K=00000%%i
            python plot_3d.py %L% %%f %DEMO_DIR%\%%~nf.!K:~-3!.png %M% %D3_ELEV% !AZIM!
            SET /A AZIM=!AZIM!+%D3_AZIM_FDELTA%
        )
    )
) ELSE (
    GOTO INVALID_DIMENSION
)

PUSHD %DEMO_DIR%
ECHO Creating GIF...
REM ImageMagick required
convert -delay %GIF_DELAY% +page *.png -loop %GIF_LOOP% %GIF_FILENAME%.gif
POPD

GOTO END

:USAGE
ECHO Usage: [D] [L] [N] [P] [M]
GOTO END

:INVALID_DIMENSION
ECHO Invalid dimension
GOTO END

:END
ECHO Exiting...
