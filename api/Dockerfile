FROM node:lts-alpine as dev-builder
ENV PORT=8080
ENV APPDIR=/home/node/app
# Adds file watcher package, necessary for live reloading feature
RUN apk add --update --no-cache inotify-tools
WORKDIR ${APPDIR}
COPY package.json yarn.lock ./
# Installs deps
RUN yarn install --frozen-lockfile
COPY nodemon.json ormconfig.yml tsconfig.json .eslintrc.yml .prettierrc.yaml ./
COPY ./src ./src/
CMD ts-node src/index.ts

FROM node:lts-alpine as builder
RUN yarn tsc
CMD node dist/index.js

FROM node:lts-alpine
ENV PORT=8080
ENV APPDIR=/home/node/app
ENV NODE_ENV=production
WORKDIR ${APPDIR}
COPY package.json yarn.lock ./
RUN yarn install --production
COPY ormconfig.yml ./
COPY --from=builder ${APPDIR}/dist ${APPDIR}/src/
USER node
CMD node src/index.js
