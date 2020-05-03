FROM node:lts-alpine as builder

ENV PORT=8080
ENV APPDIR=/home/node/app

# Adds file watcher package, necessary for live reloading feature
RUN apk add --no-cache inotify-tools

WORKDIR ${APPDIR}
COPY package.json yarn.lock ./
# Installs deps
RUN yarn install --frozen-lockfile
COPY nodemon.json ormconfig.yml tsconfig.json ./
COPY ./src ./src/
RUN yarn tsc
USER node
CMD node dist/index.js


FROM node:lts-alpine
ENV PORT=8080
ENV APPDIR=/home/node/app
WORKDIR ${APPDIR}
COPY package.json yarn.lock ./
RUN yarn install --production
COPY ormconfig.yml ./
COPY --from=builder ${APPDIR}/dist ${APPDIR}/dist/
USER node
CMD node dist/index.js
