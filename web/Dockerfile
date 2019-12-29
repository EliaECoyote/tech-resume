FROM gcr.io/tech-resume/bs-node:latest

ENV APPDIR=/home/node/app

WORKDIR ${APPDIR}
# Installs gatsby dependencies
RUN apk add --no-cache  util-linux
COPY package.json yarn.lock ./
RUN yarn install --frozen-lockfile
COPY nodemon.json bsconfig.json .eslintrc jest.config.js babel.config.js gatsby-browser.js gatsby-config.js gatsby-node.js gatsby-ssr.js firebase.json .firebaserc ./
COPY ./src ./src/
COPY ./scripts ./scripts/
# Link bs-platform
RUN npm link "bs-platform" && \
  # Bucklescript build
  yarn bsb -make-world && \
  # Gatsby build
  yarn gatsby build
USER node