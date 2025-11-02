# Build stage
FROM gcc:12 AS build

# Install build dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    libssl-dev \
    libpoco-dev \
    libmariadb-dev \
    libyaml-cpp-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/structbx

# Copy source code
COPY . .

# Build application
RUN mkdir build && cd build && \
    cmake ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/ && \
    cmake --build . --parallel $(nproc) && \
    cmake --build . --target install

# Production stage
FROM debian:12-slim

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libssl-dev \
    libpoco-dev \
    libmariadb-dev \
    libyaml-cpp-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Create necessary directories
RUN mkdir -p /etc/structbx /var/www/structbx-web /var/www/structbx-web-uploaded /var/log/structbx /var/lib/structbx

# Clone and prepare web interface
RUN git clone https://github.com/structbx/structbx-web.git /var/www/structbx-web

# Copy built application from build stage
COPY --from=build /usr/bin/structbx-server /usr/bin/

# Copy configuration files if they exist in source
COPY --from=build /etc/structbx/properties.yaml /etc/structbx/properties.yaml
COPY --from=build /etc/structbx/cert.pem /etc/structbx/cert.pem
COPY --from=build /etc/structbx/key.pem /etc/structbx/key.pem

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD [ "/usr/bin/structbx-server", "--health-check" ]

# Entrypoint with configuration
ENTRYPOINT ["/usr/bin/structbx-server", "--config", "/etc/structbx/properties.yaml"]