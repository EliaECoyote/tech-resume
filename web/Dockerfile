FROM gcr.io/tech-resume/bs-node:latest as builder

ARG GATSBY_API_SCHEME
ARG GATSBY_API_HOST
ENV APPDIR=/home/node/app
ENV GATSBY_API_SCHEME=$GATSBY_API_SCHEME
ENV GATSBY_API_HOST=$GATSBY_API_HOST

WORKDIR ${APPDIR}
# Installs gatsby dependencies
RUN apk add --no-cache util-linux
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
  yarn gatsby build && \
  # Setting node user as APPDIR owner
  chown -R node ${APPDIR} && \
  # Adding execute permission on bash scripts
  chmod -R +x ${APPDIR}/scripts
USER node
CMD npx http-server ./public


FROM node:10-alpine
# copy generated build & scripts from builder stage
COPY --from=builder ${APPDIR}/public ${APPDIR}/public/
COPY --from=builder ${APPDIR}/scripts ${APPDIR}/scripts/
USER node
CMD npx http-server ./public
