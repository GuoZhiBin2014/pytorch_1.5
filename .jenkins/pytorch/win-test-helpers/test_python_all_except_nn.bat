call %SCRIPT_HELPERS_DIR%\setup_pytorch_env.bat
cd test && python run_test.py --exclude test_nn test_jit_profiling test_jit_legacy test_jit_fuser_legacy test_jit_fuser_profiling --verbose --determine-from="%1" && cd ..
if ERRORLEVEL 1 exit /b 1
