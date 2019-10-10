FROM node:10-alpine

ENV APPDIR=/usr/src/app
ENV PORT=3000

ENV PANDOC_VERSION 2.7.3
ENV PANDOC_DOWNLOAD_URL https://hackage.haskell.org/package/pandoc-$PANDOC_VERSION/pandoc-$PANDOC_VERSION.tar.gz
ENV PANDOC_ROOT /usr/local/pandoc

ENV BUILD_DEPS \
    alpine-sdk \
    cabal \
    coreutils \
    ghc \
    libffi \
    musl-dev \
    zlib-dev
ENV PERSISTENT_DEPS \
    gmp \
    graphviz \
    openjdk11-jre \
    python \
    py2-pip \
    sed \
    ttf-droid \
    ttf-droid-nonlatin

ENV PATH $PATH:$PANDOC_ROOT/bin

ENV NINJA_FORCE_ANSI=1


# adds pandoc package and remove build dependencies
RUN apk upgrade --update && \
    apk add --virtual .build-deps $BUILD_DEPS && \
    apk add --virtual .persistent-deps $PERSISTENT_DEPS && \
    mkdir -p /pandoc-build \
             /var/docs && \
    cd /pandoc-build && \
    curl -fsSL "$PANDOC_DOWNLOAD_URL" | tar -xzf - && \
    cd pandoc-$PANDOC_VERSION && \
    cabal update && \
    cabal install --only-dependencies && \
    cabal configure --prefix=$PANDOC_ROOT && \
    cabal build && \
    cabal copy && \
    cd / && \
    rm -Rf /pandoc-build \
           $PANDOC_ROOT/lib \
           /root/.cabal \
           /root/.ghc && \
    set -x && \
    addgroup -g 82 -S pandoc && \
    adduser -u 82 -D -S -G pandoc pandoc && \
    apk del .build-deps



WORKDIR ${APPDIR}

RUN apk --no-cache add \
  inotify-tools \
  # bsb ninja build dependencies
  python g++ make

COPY package.json yarn.lock ./

RUN yarn install

COPY ./src ./scripts bsconfig.json ./
