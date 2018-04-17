FROM ubuntu:artful-20180412

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y libgl1-mesa-dev mesa-common-dev pkg-config ninja-build cmake python2.7 zip python3.6 xvfb

ENV HUNTER_ROOT=/hunter

WORKDIR /src

COPY CMakeLists.txt init ./
COPY cmake cmake

RUN mkdir pixel && touch pixel/CMakeLists.txt

RUN ./init

COPY . ./

RUN ./build
#
RUN ./dist

#RUN xvfb-run ./build-ninja/bin/pixel_test pixel
