#!/bin/bash

# Add permissions to xhost
xhost +local:1000

# Run docker image
docker run -it --rm \
    -v ${PWD}:/brave-2 \
    -e DISPLAY=$DISPLAY \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    bobarna/brave-2 \
 /brave-2/renders/brave2
