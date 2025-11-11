FROM ubuntu:24.04

# Install compilers + multilib for 32-bit builds
RUN dpkg --add-architecture i386 && \
    apt-get update && apt-get install -y \
    build-essential \
    gcc-multilib g++-multilib \
    make \
    && rm -rf /var/lib/apt/lists/*

# Working dir inside container
WORKDIR /build
