FROM gcc:9.2

ENV DEBIAN_FRONTEND=noninteractive

RUN set -ex; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        bison llvm-runtime llvm-dev


WORKDIR /app
COPY . .

RUN make
