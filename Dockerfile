FROM thewtex/opengl:latest

RUN apt-get update
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add && \
    sudo add-apt-repository -y "deb http://apt.llvm.org/jessie/ llvm-toolchain-jessie-5.0 main"

RUN apt-get install -y libgl1-mesa-dev mesa-common-dev pkg-config ninja-build python2.7 zip python3.4 clang-5.0

RUN wget https://cmake.org/files/v3.9/cmake-3.9.6-Linux-x86_64.tar.gz && \
    tar xf cmake-3.9.6-Linux-x86_64.tar.gz && \
    cp cmake-3.9.6-Linux-x86_64/bin/* /usr/local/bin && \
    cp -r cmake-3.9.6-Linux-x86_64/share/cmake-3.9/ /usr/local/share/

ENV HUNTER_ROOT=/hunter

#VOLUME ["/hunter"]

WORKDIR /src

COPY CMakeLists.txt init ./
COPY cmake cmake

RUN mkdir pixel && touch pixel/CMakeLists.txt && ./init > /dev/null

COPY . ./

#RUN ./build
#
#RUN ./dist

#RUN ./build-ninja/bin/pixel_test pixel
