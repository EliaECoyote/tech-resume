API_DC_FILE = api/docker-compose.yml

.PHONY: start

start-api:
	docker-compose -f ${API_DC_FILE} up

build-api:
	docker-compose -f ${API_DC_FILE} build

connect-api:
	docker-compose -f ${API_DC_FILE} run api /bin/sh
