FROM ubuntu:17.10

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y libgl1-mesa-dev mesa-common-dev pkg-config ninja-build cmake python2.7

COPY cmake.init CMakeLists.txt
COPY cmake cmake
COPY init run test ./
RUN ./init
COPY pixel pixel/
COPY CMakeLists.txt ./
RUN ./init
