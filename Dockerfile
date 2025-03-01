FROM debian:12 AS build

RUN apt-get update && apt-get install -y \
    make \
    binutils \
    g++ \
    git \
    cmake \
    libssl-dev \
    libpoco-dev \
    libmariadb-dev \
    libyaml-cpp-dev

WORKDIR /usr/src/structbx

COPY . /usr/src/structbx

RUN mkdir build && cd build && \
    cmake ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/structbx && \
    cmake --build . && \
    cmake --build . --target install && \
    git clone https://github.com/structbx/structbx-web.git

FROM debian:12

COPY --from=build /structbx /

COPY --from=build /usr/src/structbx/build/structbx-web /var/www/structbx-web

WORKDIR /

ENTRYPOINT ["/bin/structbx-server", "--config", "/etc/structbx/properties.yaml"]