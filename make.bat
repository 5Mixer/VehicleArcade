@if exist "%~dp0Kore\Kinc\Tools\windows_x64\kmake.exe" (
	@call "%~dp0Kore\Kinc\Tools\windows_x64\kmake.exe" %*
) else (
	echo kmake was not found, please run the get_dlc script.
)