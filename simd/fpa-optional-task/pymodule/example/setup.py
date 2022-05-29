from distutils.core import setup, Extension

def main():
    setup(
        name="simdcount", 
        ext_modules=[
            Extension(
                "simdcount", 
                [
                    "simdcount.c"
                ]
            )
        ]
    )

if __name__ == "__main__":
    main()
