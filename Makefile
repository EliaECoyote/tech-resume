DC_FILE = docker-compose.yml

.PHONY: start

install:
	cd ${PWD}/api && yarn install; \
	cd ${PWD}/web && yarn install;

start:
	docker-compose -f ${DC_FILE} up	

build:
	docker-compose -f ${DC_FILE} build

connect-api:
	docker-compose -f ${DC_FILE} run api /bin/sh

connect-web:
	docker-compose -f ${DC_FILE} run web /bin/sh