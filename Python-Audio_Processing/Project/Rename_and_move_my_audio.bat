echo off
SETLOCAL ENABLEDELAYEDEXPANSION
mkdir my_audio
set /A i = 0
for /F "tokens=*" %%u in (audio.txt) do  (
    ROBOCOPY .\audio_files .\my_audio\ %%u /mt /mov	
	set name=!i!_%%u
	cd my_audio\
	ren %%u !name!	
	cd ..
	set /A i = !i! + 1
)

ENDLOCAL
pause