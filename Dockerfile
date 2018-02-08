FROM ubuntu:17.10

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y libgl1-mesa-dev mesa-common-dev pkg-config ninja-build cmake python2.7

COPY . ./
RUN ./init
