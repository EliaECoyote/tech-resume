FROM node:10-alpine

ENV APPDIR=/usr/src/app
ENV PORT=3000

WORKDIR ${APPDIR}

RUN apk --no-cache add \
  inotify-tools \
  # bsb ninja build dependencies
  python g++ make

COPY package.json yarn.lock ./

RUN yarn install

COPY ./src ./scripts bsconfig.json ./
