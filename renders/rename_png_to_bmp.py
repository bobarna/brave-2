#!/usr/bin/python3

# from pathlib import Path
# import os
# import sys
import pathlib

for path in pathlib.Path(".").iterdir():
    if path.is_file() and path.suffix == "png":
        old_name = path.stem
        old_extension = path.suffix
        directory = path.parent
        new_name = old_name + ".bmp"

        path.rename(pathlib.Path(directory, new_name))
