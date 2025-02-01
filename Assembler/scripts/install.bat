@echo off
:: Clone the repository
git clone https://github.com/your-username/my-assembler.git
cd my-assembler

:: Check if the user has MinGW or another compiler installed (optional)
where gcc >nul 2>&1
if errorlevel 1 (
    echo "GCC not found. Please install MinGW or another C compiler."
    exit /b 1
)

:: Build the project using gcc or make (if available)
echo "Building the assembler..."
gcc -o build\my-assembler src\main.c src\cli\cli.c src\assembler\*.c -Iinclude

:: Check if the build was successful
if exist build\my-assembler.exe (
    echo "Build successful!"
) else (
    echo "Build failed. Exiting."
    exit /b 1
)

:: Move the executable to a directory in the system PATH
echo "Installing my-assembler to C:\Program Files\my-assembler..."
if not exist "C:\Program Files\my-assembler" mkdir "C:\Program Files\my-assembler"
copy build\my-assembler.exe "C:\Program Files\my-assembler\"

:: Add the path to the system PATH if not already there (optional step)
set "newPath=C:\Program Files\my-assembler"
echo %PATH% | findstr /I /C:"%newPath%" >nul
if %errorlevel%==0 (
    echo "Path already exists in system PATH."
) else (
    echo "Adding %newPath% to system PATH."
    setx PATH "%newPath%;%PATH%"
)

echo "Installation complete. You can now run 'my-assembler' from the command line."
