from distutils.core import setup, Extension
import os


if __name__ == "__main__":
    setup(
        name = "pysimd", 
        ext_modules = [
            Extension(
                "pysimd", [
                    "pysimd.c"
                ]
            )
        ]
    )
