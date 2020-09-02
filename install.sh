#!/bin/bash

mkdir tmp && \
cd tmp && \
wget --no-verbose https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz && \
tar xzf boost_1_65_1.tar.gz && \
cd boost_1_65_1 && \
sudo ./bootstrap.sh --with-libraries=python --with-python=$(which python3)&& \
sudo ./b2 install && \
cd .. && \
sudo rm -rf tmp