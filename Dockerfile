FROM ubuntu:17.10

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y libgl1-mesa-dev mesa-common-dev pkg-config ninja-build cmake python2.7 zip python3.6

ENV HUNTER_ROOT=/hunter

VOLUME ["/hunter"]

WORKDIR /src

COPY CMakeLists.txt init ./
COPY cmake cmake

RUN mkdir pixel && touch pixel/CMakeLists.txt && ./init > /dev/null

COPY . ./

#RUN ./build
#
#RUN ./dist

#RUN ./build-ninja/bin/pixel_test pixel
