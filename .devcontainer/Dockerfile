FROM mcr.microsoft.com/devcontainers/cpp:1-debian

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

RUN echo "Package: *" > /etc/apt/preferences.d/testing \
    && echo "Pin: release a=testing" >> /etc/apt/preferences.d/testing \
    && echo "Pin-Priority: 100" >> /etc/apt/preferences.d/testing \
    && echo "deb http://deb.debian.org/debian testing main" > /etc/apt/sources.list.d/testing.list \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        -t testing \
        gcc-13 \
        g++-13 \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        gdb \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 130 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 130 \
    && rm -rf /var/lib/apt/lists/*
    
    
    
# Install UV
RUN curl -LsSf https://astral.sh/uv/install.sh | env UV_UNMANAGED_INSTALL="/usr/local/bin" sh
# Install just
RUN curl -fsSL https://just.systems/install.sh | bash -s -- --to /usr/local/bin


