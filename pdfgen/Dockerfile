FROM bs-node

ENV APPDIR=/usr/src/app

WORKDIR ${APPDIR}

RUN apk add --update --no-cache \
  libgcc libstdc++ libx11 glib libxrender libxext libintl \
  ttf-dejavu ttf-droid ttf-freefont ttf-liberation ttf-ubuntu-font-family

COPY --from=madnight/alpine-wkhtmltopdf-builder:0.12.5-alpine3.10-606718795 \
  /bin/wkhtmltopdf /bin/wkhtmltopdf

COPY package.json yarn.lock ./

RUN yarn install

COPY ./src ./scripts nodemon.json bsconfig.json ./
COPY ./src ./src/
COPY ./scripts ./scripts/
