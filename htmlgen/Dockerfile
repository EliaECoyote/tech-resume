FROM eliaecoyote/alpine-bucklescript:7

ENV PORT=8080
ENV APPDIR=/home/node/app

# Adds file watcher package, necessary for live reloading feature
RUN apk add --no-cache inotify-tools

WORKDIR ${APPDIR}
COPY package.json yarn.lock ./
# Installs deps
RUN yarn install --frozen-lockfile
COPY nodemon.json bsconfig.json jest.config.js ./
COPY ./__tests__ ./__tests__/
COPY ./src ./src/
COPY ./scripts ./scripts/
# Link bs-platform and builds project
RUN npm link "bs-platform" \
  && yarn bsb -make-world \
  # Setting node user as APPDIR owner
  && chown -R node ${APPDIR} \
  # Adding execute permission on bash scripts
  && chmod -R +x ${APPDIR}/scripts
USER node
CMD node lib/js/src/index.bs.js
