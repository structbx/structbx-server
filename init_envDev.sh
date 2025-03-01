#!/bin/bash

BASEDIR="$PWD"

mkdir -p "$BASEDIR/build/Debug/DockerVolume" && \
cd "$BASEDIR/build/Debug" && \
conan install ../../ --output-folder=./ --build=missing && \
openssl req -nodes -new -x509 -keyout key.pem -out cert.pem && \
cd "$BASEDIR" && \
cp conf/properties.yaml.template build/Debug/properties.yaml && \
cp conf/properties.yaml.template build/Debug/DockerVolume/properties.yaml && \
cp build/Debug/*.pem build/Debug/DockerVolume/
