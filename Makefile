DC_FILE = docker-compose.yml

.PHONY: start build install connect-api connect-web

start:
	docker-compose -f ${DC_FILE} up	

build:
	docker-compose -f ${DC_FILE} build

clean:
	cd ${PWD}/api && yarn clean; \
	cd ${PWD}/web && yarn clean;

install:
	cd ${PWD}/api && yarn install; \
	cd ${PWD}/web && yarn install;

connect-api:
	docker-compose -f ${DC_FILE} run api /bin/sh

connect-web:
	docker-compose -f ${DC_FILE} run web /bin/sh
