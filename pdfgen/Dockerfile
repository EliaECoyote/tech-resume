FROM gcr.io/tech-resume/bs-node:latest

# ******************************
# Inspired by *Running puppeteer on alpine* section in the puppeter docs: 
# https://github.com/puppeteer/puppeteer/blob/master/docs/troubleshooting.md#running-puppeteer-in-docker
# ******************************

ENV APPDIR=/home/node/app
# Tell Puppeteer to skip installing Chrome. We'll be using the installed package
ENV PUPPETEER_SKIP_CHROMIUM_DOWNLOAD true
ENV PORT=8080

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
# Installs deps
RUN yarn install --frozen-lockfile
COPY nodemon.json bsconfig.json ./
COPY ./src ./src/
COPY ./scripts ./scripts/

# Link bs-platform and builds project
RUN npm link "bs-platform" \
  && yarn bsb -make-world \
  # Add user so we don't need --no-sandbox
  && addgroup -S pptruser \
  && adduser -S -g pptruser pptruser \
  && mkdir -p /home/pptruser/Downloads /app \
  && chown -R pptruser:pptruser /home/pptruser \
  && chown -R pptruser:pptruser /app \
  # Setting pptruser user as APPDIR owner
  && chown -R pptruser:pptruser ${APPDIR} \
  # Adding execute permission on bash scripts
  && chmod -R +x ${APPDIR}/scripts
USER pptruser
CMD node lib/js/src/index.bs.js
