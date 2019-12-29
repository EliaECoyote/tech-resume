ARG BS_PLATFORM_VERSION


FROM node:10-alpine
# Adds bsb ninja build dependencies & bs-platform package
RUN apk add g++ make python && \
  npm install -g bs-platform@${BS_PLATFORM_VERSION} --unsafe-perm


FROM node:lts-alpine
ENV GLOBAL_NODE_MODULES_PATH=/usr/local/lib/node_modules
# Adds file watcher package, necessary for live reloading feature
RUN apk add --no-cache inotify-tools
# Copies bs-platform package from builder image
COPY --from=0 ${GLOBAL_NODE_MODULES_PATH} ${GLOBAL_NODE_MODULES_PATH}