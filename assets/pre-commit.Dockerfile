FROM alpine:latest

RUN apk add clang clang-extra-tools
WORKDIR /mnt

CMD clang-format -i src/*.cpp src/*/*.cpp src/*/*/*.cpp inc/*.hpp