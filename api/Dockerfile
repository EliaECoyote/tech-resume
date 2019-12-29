FROM gcr.io/tech-resume/bs-node:latest

ENV APPDIR=/home/node/app
ENV DEBUG_PORT=9229

WORKDIR ${APPDIR}
COPY package.json yarn.lock ./
# install deps
RUN yarn install --frozen-lockfile
COPY nodemon.json bsconfig.json ./
COPY ./src ./src/
COPY ./scripts ./scripts/
RUN   echo "yarn global" && \
  ls -la /usr/local/share/.config/yarn/global && \
  echo "yarn global node modules" && \
  ls -la /usr/local/share/.config/yarn/global/node_modules && \
  cd /usr/local/share/.config/yarn/global/node_modules/bs-platform && \
  yarn link && \
  cd ${APPDIR} && \
  yarn link "bs-platform"
RUN yarn bsb -make-world
USER node
CMD node --inspect=$DEBUG_PORT lib/js/src/index.bs.js
