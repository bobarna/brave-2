FROM ubuntu:focal

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    echo $TZ > /etc/timezone && \
    apt-get update && apt-get install -y \
        build-essential \
        cmake \
        libglfw3-dev \
        libglew-dev
