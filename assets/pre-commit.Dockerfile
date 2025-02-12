FROM alpine:latest

RUN apk add clang clang-extra-tools
WORKDIR /mnt

CMD clang-format -i --style=LLVM /mnt/src/*.cpp /mnt/inc/*.h