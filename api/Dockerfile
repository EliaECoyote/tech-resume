FROM bs-node

ENV APPDIR=/home/node/app
ENV DEBUG_PORT=9229

WORKDIR ${APPDIR}

COPY package.json yarn.lock ./

RUN npm link bs-platform && yarn install --frozen-lockfile

COPY nodemon.json bsconfig.json ./
COPY ./src ./src/
COPY ./scripts ./scripts/

USER node

RUN npm run bsb -make-world

CMD node --inspect=$DEBUG_PORT lib/js/src/index.bs.js
