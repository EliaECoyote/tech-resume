FROM node:10-alpine

ENV APPDIR=/usr/src/app
ENV PORT=3000

WORKDIR ${APPDIR}

COPY package.json yarn.lock ./

RUN apk add --no-cache \
  # File watcher package, necessary for live reloading
  inotify-tools \
  # bsb ninja build dependencies
  python g++ make \
  # gatsby dependencies
  util-linux

RUN yarn install
COPY ./src ./scripts bsconfig.json .eslintrc jest.config.js babel.config.js gatsby-browser.js gatsby-config.js gatsby-node.js gatsby-ssr.js ./
