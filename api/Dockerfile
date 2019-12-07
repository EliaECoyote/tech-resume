FROM bs-node

ENV APPDIR=/usr/src/app

WORKDIR ${APPDIR}

COPY package.json yarn.lock ./

RUN yarn install

COPY ./src ./styles ./scripts nodemon.json bsconfig.json ./
