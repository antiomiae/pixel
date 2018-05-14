FROM ubuntu:18.04

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y libgl1-mesa-dev mesa-common-dev pkg-config ninja-build cmake zip python3.6 python2.7 build-essential

ENV HUNTER_ROOT=/hunter

WORKDIR /src

COPY CMakeLists.txt init ./
COPY cmake cmake

RUN mkdir pixel && touch pixel/CMakeLists.txt

RUN ./init

COPY . ./

RUN ./init && ./build
#
RUN ./dist

#RUN xvfb-run ./build-ninja/bin/pixel_test pixel
