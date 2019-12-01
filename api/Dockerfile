FROM node:10-alpine

ENV APPDIR=/usr/src/app
ENV PORT=3000

WORKDIR ${APPDIR}

COPY package.json yarn.lock ./

RUN apk add --no-cache \
  # File watcher package, necessary for live reloading
  inotify-tools \
  # bsb ninja build dependencies
  python g++ make

RUN yarn install
COPY ./src ./styles ./scripts nodemon.json bsconfig.json ./
