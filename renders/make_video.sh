ffmpeg -framerate 24 -i render%04d.bmp output.mp4

find . -type f -iname \*.bmp -delete

