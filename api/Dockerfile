# start from node image so we can install esy from npm
FROM node:alpine as esy-node

ENV TERM=dumb LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:/lib

RUN mkdir /esy
WORKDIR /esy

ENV NPM_CONFIG_PREFIX=/esy
RUN npm install -g --unsafe-perm esy

# now that we have esy installed we need a proper runtime

FROM alpine as esy-alpine

ENV TERM=dumb LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:/lib

WORKDIR /

COPY --from=esy-node /esy /esy

RUN apk add --no-cache ca-certificates wget bash curl perl-utils git patch gcc g++ musl-dev make m4

RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
RUN wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.28-r0/glibc-2.28-r0.apk
RUN apk add --no-cache glibc-2.28-r0.apk

ENV PATH=/esy/bin:$PATH


FROM esy-alpine as builder

ENV PORT=8080
ENV APPDIR=/home/node/app

# Adds file watcher package, necessary for live reloading feature, & opam
RUN apk add --no-cache inotify-tools opam

WORKDIR ${APPDIR}
COPY ./esy.lock ./esy.lock
COPY api.opam dune dune-project package.json ./
# Install deps
RUN esy install
COPY ./executable ./executable
COPY ./library ./library
COPY ./test ./test
# Build
RUN esy build
CMD esy x ApiApp.exe