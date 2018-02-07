FROM ubuntu:17.10

RUN apt-get update && apt-get upgrade -y

RUN apt-get install -y libgl1-mesa-dev pkg-config

COPY . src

RUN cd src && ./init

