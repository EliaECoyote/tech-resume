ARG BS_PLATFORM_VERSION


FROM node:10-alpine
# Adds bsb ninja build dependencies & bs-platform package
RUN apk add g++ make python && \
   yarn global add bs-platform@${BS_PLATFORM_VERSION}


FROM node:lts-alpine
# Adds file watcher package, necessary for live reloading feature
RUN apk add --no-cache inotify-tools
# Copies bs-platform package from builder image
COPY --from=0 /usr/local/share/.config/yarn/global /usr/local/share/.config/yarn/global
RUN cd /usr/local && \
  # creates symlink of bucklescript binaries
  ln -s ../share/.config/yarn/global/node_modules/.bin/bsb bin/bsb && \
  ln -s ../share/.config/yarn/global/node_modules/.bin/bsc bin/bsc && \
  ln -s ../share/.config/yarn/global/node_modules/.bin/bsrefmt bin/bsrefmt && \
  bsb -v && \
  bsc -v
CMD ["sh"]