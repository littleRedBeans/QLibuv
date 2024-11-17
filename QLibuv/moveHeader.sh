#!/bin/bash
source_path="$1"
mkdir -p "$source_path"/../include/QLibuv
cp -u "$source_path"/UvUdpSocket.h "$source_path"/../include/QLibuv
