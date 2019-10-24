DC_FILE = docker-compose.yml

.PHONY: start build install connect-api connect-web

start:
	docker-compose -f ${DC_FILE} up	

build:
	docker-compose -f ${DC_FILE} build

install:
	cd ${PWD}/api && yarn install; \
	cd ${PWD}/web && yarn install;

connect-api:
	docker-compose -f ${DC_FILE} run api /bin/sh

connect-web:
	docker-compose -f ${DC_FILE} run web /bin/sh

ps-api:
	docker top markdown2cv_api_1

ps-web:
	docker top markdown2cv_web_1
