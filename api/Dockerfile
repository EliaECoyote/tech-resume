FROM node:10-alpine

# Testing: NINJA_FORCE_ANSI env should enable colors for concurrently package
ENV NINJA_FORCE_ANSI=1
ENV APPDIR=/usr/src/app
ENV PORT=3000

WORKDIR ${APPDIR}

COPY --from=pandoc/core /usr/bin/* /usr/bin/
COPY package.json yarn.lock ./

RUN apk add --no-cache \
  lua5.3 \
  # File watcher package, necessary for live reloading
  inotify-tools \
  # bsb ninja build dependencies
  python g++ make
RUN yarn install
COPY ./src ./scripts bsconfig.json ./
