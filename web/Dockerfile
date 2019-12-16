FROM bs-node

ENV APPDIR=/usr/src/app

WORKDIR ${APPDIR}

RUN apk add --no-cache \
  # gatsby dependencies
  util-linux

COPY package.json yarn.lock ./

RUN yarn install

COPY nodemon.json bsconfig.json .eslintrc jest.config.js babel.config.js gatsby-browser.js gatsby-config.js gatsby-node.js gatsby-ssr.js ./
COPY ./src ./src/
COPY ./scripts ./scripts/
