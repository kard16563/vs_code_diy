@rem This script invokes the most recently built Python with all arguments
@rem passed through to the interpreter.  This file is generated by the
@rem build process and any changes *will* be thrown away by the next
@rem rebuild.
@rem This is only meant as a convenience for developing CPython
@rem and using it outside of that context is ill-advised.
@echo Running Debug^|Win32 interpreter...
@setlocal
@set PYTHONHOME=E:\the_c_of_vs_code\c\c_study\12\Python-3.8.0\

@"E:\the_c_of_vs_code\c\c_study\12\Python-3.8.0\PCbuild\win32\python_d.exe" %*
