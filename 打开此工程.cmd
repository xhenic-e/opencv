@echo off
rem Open this folder as a normal (single-folder) VSCode workspace.
rem F5 then uses .vscode\launch.json directly and stops asking you to pick a config.
setlocal
for %%I in ("%~dp0.") do set "HERE=%%~fI"
start "" "D:\vscode\Microsoft VS Code\Code.exe" "%HERE%"
