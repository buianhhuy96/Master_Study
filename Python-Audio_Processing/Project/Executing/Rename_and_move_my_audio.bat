rem "This file will choose my annotation audio to be renamed and copied to" 
rem "processing folder."

echo off
SETLOCAL ENABLEDELAYEDEXPANSION
mkdir my_audio
set /A i = 0
rem "get choosen audio files listed in my "audio.txt""
for /F "tokens=*" %%u in (audio.txt) do  (
	rem "copy from .\audio_files to .\my_audio\ so that my code can run
    ROBOCOPY .\audio_files .\my_audio\ %%u /mt /mov	
	rem "define new name for audio"
	set name=!i!_%%u
	rem "move into destination folder to rename the file"
	cd my_audio\
	ren %%u !name!	
	rem "move out to parent foler"
	cd ..
	rem "increment counter"
	set /A i = !i! + 1
)

ENDLOCAL
pause