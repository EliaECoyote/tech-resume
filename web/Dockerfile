FROM gcr.io/tech-resume/bs-node:latest as builder

ARG GATSBY_API_SCHEME
ARG GATSBY_API_HOST
ARG GATSBY_PDFGEN_SCHEME
ARG GATSBY_PDFGEN_HOST

ENV GATSBY_API_SCHEME=$GATSBY_API_SCHEME
ENV GATSBY_API_HOST=$GATSBY_API_HOST
ENV GATSBY_PDFGEN_SCHEME=$GATSBY_PDFGEN_SCHEME
ENV GATSBY_PDFGEN_HOST=$GATSBY_PDFGEN_HOST
ENV APPDIR=/home/node/app
# As described by gatsby docs
# https://www.gatsbyjs.org/docs/environment-variables/#build-variables
# setting CI=true as an environment variable will tailor output for dumb terminals
ENV CI=true


WORKDIR ${APPDIR}
# Installs gatsby dependencies
RUN apk add --no-cache util-linux
COPY package.json yarn.lock ./
RUN yarn install --frozen-lockfile
COPY nodemon.json bsconfig.json .eslintrc jest.config.js babel.config.js gatsby-browser.js gatsby-config.js gatsby-node.js gatsby-ssr.js firebase.json .firebaserc ./
COPY ./src ./src/
COPY ./scripts ./scripts/
# Link bs-platform
RUN npm link "bs-platform" \
  # Bucklescript build
  && yarn bsb -make-world \
  # Gatsby build
  && yarn gatsby build \
  # Setting node user as APPDIR owner
  && chown -R node ${APPDIR} \
  # Adding execute permission on bash scripts
  && chmod -R +x ${APPDIR}/scripts
USER node
CMD npx http-server ./public


FROM node:10-alpine
ENV APPDIR=/home/node/app
WORKDIR ${APPDIR}
RUN npm install -g firebase-tools
# copy generated build & config from builder stage
COPY --from=builder ${APPDIR}/public ${APPDIR}/public/
COPY --from=builder ${APPDIR}/firebase.json ${APPDIR}/.firebaserc ./
USER node
CMD npx http-server ./public
