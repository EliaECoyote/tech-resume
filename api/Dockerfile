# Stage that builds pandoc
FROM alpine AS pandoc-builder
# Install pandoc deps
RUN apk --no-cache add \
  alpine-sdk \
  bash \
  ca-certificates \
  cabal \
  fakeroot \
  ghc \
  git \
  gmp-dev \
  lua5.3-dev \
  pkgconfig \
  zlib-dev
# Install newer cabal-install version
COPY cabal.root.config /root/.cabal/config
RUN cabal update \
  && cabal install cabal-install \
  && mv /root/.cabal/bin/cabal /usr/local/bin/cabal
# Get pandoc sources
ARG pandoc_commit=master
RUN git clone --branch=$pandoc_commit --depth=1 --quiet \
  https://github.com/jgm/pandoc /usr/src/pandoc
# Install Haskell deps
WORKDIR /usr/src/pandoc
RUN cabal --version \
  && ghc --version \
  && cabal new-update \
  && cabal new-clean \
  && cabal new-configure \
      --flag embed_data_files \
      --flag bibutils \
      --constraint 'hslua +system-lua +pkg-config' \
      --enable-tests \
      . pandoc-citeproc \
  && cabal new-build . pandoc-citeproc


# Stage that extracts pandoc binaries
FROM pandoc-builder AS pandoc-binaries
RUN find dist-newstyle \
  -name 'pandoc*' -type f -perm +400 \
  -exec cp '{}' /usr/bin/ ';' \
  && strip /usr/bin/pandoc /usr/bin/pandoc-citeproc


# Final stage
FROM node:10-alpine

# Testing: NINJA_FORCE_ANSI env should enable colors for concurrently package
ENV NINJA_FORCE_ANSI=1
ENV APPDIR=/usr/src/app
ENV PORT=3000

WORKDIR ${APPDIR}

COPY --from=pandoc-binaries /usr/bin/pandoc* /usr/bin/
COPY package.json yarn.lock ./

RUN apk --no-cache add \
  # File watcher package, necessary for live reloading
  inotify-tools \
  # bsb ninja build dependencies
  python g++ make
RUN yarn install
COPY ./src ./scripts bsconfig.json ./
