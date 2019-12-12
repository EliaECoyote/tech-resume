FROM bs-node

ENV APPDIR=/usr/src/app

WORKDIR ${APPDIR}

COPY package.json yarn.lock ./

RUN yarn install

COPY nodemon.json bsconfig.json ./
COPY ./src ./src/
COPY ./scripts ./scripts/
