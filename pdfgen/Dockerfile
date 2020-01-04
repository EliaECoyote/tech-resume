FROM gcr.io/tech-resume/bs-node:latest

ARG SCHEME
ARG HOST
ARG PORT

# ******************************
# Inspired by *Running puppeteer on alpine* section in the puppeter docs: 
# https://github.com/puppeteer/puppeteer/blob/master/docs/troubleshooting.md#running-puppeteer-in-docker
# ******************************

ENV APPDIR=/home/node/app
# Tell Puppeteer to skip installing Chrome. We'll be using the installed package
ENV PUPPETEER_SKIP_CHROMIUM_DOWNLOAD true
ENV SCHEME=$SCHEME
ENV HOST=$HOST
ENV PORT=$PORT

# Installs latest Chromium (77) package
RUN apk add --no-cache \
  chromium \
  nss \
  freetype \
  freetype-dev \
  harfbuzz \
  ca-certificates \
  ttf-freefont
# Puppeteer v1.19.0 works with Chromium 77
RUN yarn add puppeteer@1.19.0
WORKDIR ${APPDIR}
COPY package.json yarn.lock ./
RUN yarn install
COPY nodemon.json bsconfig.json ./
COPY ./src ./src/
COPY ./scripts ./scripts/
# Add user so we don't need --no-sandbox
RUN addgroup -S pptruser && adduser -S -g pptruser pptruser \
  && mkdir -p /home/pptruser/Downloads /app \
  && chown -R pptruser:pptruser /home/pptruser \
  && chown -R pptruser:pptruser /app \
  && chown -R pptruser:pptruser ${APPDIR}
# Run everything after as non-privileged user
USER pptruser
CMD node lib/js/src/index.bs.js
