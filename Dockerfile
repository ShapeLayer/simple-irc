FROM gcc:latest

WORKDIR /root
RUN mkdir temp
WORKDIR /root/temp
RUN curl -OL https://github.com/Kitware/CMake/releases/download/v4.2.1/cmake-4.2.1.tar.gz
RUN tar -xzvf cmake-4.2.1.tar.gz

WORKDIR /root/temp/cmake-4.2.1
RUN ./bootstrap -- -DCMAKE_BUILD_TYPE:STRING=Release
RUN make -j4
RUN make install


WORKDIR /usr/src/app
COPY ./server/include ./server/include
COPY ./server/src ./server/src
COPY ./server/CMakeLists.txt ./server/CMakeLists.txt
COPY ./include ./include

WORKDIR /usr/src/app/server
RUN mkdir build
WORKDIR /usr/src/app/server/build
RUN cmake .. 
RUN make

EXPOSE 6667

CMD ["/usr/src/app/server/build/simple_irc_server"]
