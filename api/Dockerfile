#
# Ubuntu Node.js Dockerfile
#
# https://github.com/dockerfile/ubuntu/blob/master/Dockerfile
#
FROM ubuntu:18.04

# Testing: NINJA_FORCE_ANSI env should enable colors for concurrently package
ENV NINJA_FORCE_ANSI=1
ENV APPDIR=/usr/src/app
ENV PORT=3000
# Node Enviroment variables
ENV NODE_VERSION 10.17.0
ENV YARN_VERSION 1.17.3
ENV NODE_ARCH x64
ENV TMP /tmp
ENV NODE_FILEPATH node-v$NODE_VERSION-linux-$NODE_ARCH

WORKDIR ${APPDIR}
RUN mkdir -p $APPDIR

# Udpating and Installing dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
  ca-certificates \
  curl \
  xz-utils \
  openssl \
  && rm -rf /var/lib/apt/lists/*

# Install Nodejs
RUN curl -SL https://nodejs.org/dist/v$NODE_VERSION/$NODE_FILEPATH.tar.xz -o $TMP/$NODE_FILEPATH.tar.xz \
  && cd $TMP/ \
  && tar -xJf $NODE_FILEPATH.tar.xz \
  && rm $NODE_FILEPATH.tar.xz \
  && mv $NODE_FILEPATH /opt/node \
  && ln -sf /opt/node/bin/node /usr/bin/node \
  && ln -sf /opt/node/bin/npm /usr/bin/npm

# Install Yarn
RUN curl -fSLO --compressed "https://yarnpkg.com/downloads/$YARN_VERSION/yarn-v$YARN_VERSION.tar.gz" \
  && tar -xzf yarn-v$YARN_VERSION.tar.gz -C /opt/ \
  && ln -snf /opt/yarn-v$YARN_VERSION/bin/yarn /usr/local/bin/yarn \
  && ln -snf /opt/yarn-v$YARN_VERSION/bin/yarnpkg /usr/local/bin/yarnpkg \
  && rm yarn-v$YARN_VERSION.tar.gz

COPY package.json yarn.lock ./

RUN apt-get update && \
  apt-get install -y \
  build-essential \
  wget \
  context \
  && rm -rf /var/lib/apt/lists/*
RUN wget https://github.com/jgm/pandoc/releases/download/2.2.1/pandoc-2.2.1-1-amd64.deb
RUN dpkg -i pandoc-2.2.1-1-amd64.deb  && rm pandoc-*.deb

# Cleanup to reduce image size
RUN apt-get remove -y wget && \ 
  apt-get autoclean && \
  apt-get clean

RUN yarn install
COPY ./src ./styles ./scripts bsconfig.json ./
