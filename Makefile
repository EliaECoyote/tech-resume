DC_FILE = docker-compose.yml

API_FOLDER = api
WEB_FOLDER = web

API_SERVICE = api
WEB_SERVICE = web
PDFGEN_SERVICE = pdfgen
SERVICE =

define HELP_MESSAGE

  Syntax:
  make [target] [action]
  
  **Targets**
  - api
  - web
  - pdfgen
  
  **Actions**
  - help          prints make cmds docs
  - install       install modules dependencies
  - serve         starts development server on loopback address
  - test          runs tests in a new container. Requires a target
  - test-watch    runs tests (watch mode) in a new container. Requires a target
  - connect       starts a bash shell in a new container. Requires a target
  - build-image   rebuilds docker images
  - deploy        builds and deploys the service
  
  When [target] is not provided, the [action] will run in all modules by default
  e.g.: 
  ```
    # runs development servers for both *api* & *web* services.
    make serve
  ```

  Otherwise, the action will only run in the specified target module
  e.g.:
  ```
    # runs tests for the *web* service
    make web test
  ```

endef
export HELP_MESSAGE

help: 
	@echo "$$HELP_MESSAGE";

# Services (targets)
api:
	@echo "\n** running in service: api **\n"
	$(eval SERVICE=$(API_SERVICE))
web:
	@echo "\n** running in service: web **\n"
	$(eval SERVICE=$(WEB_SERVICE))
pdfgen:
	@echo "\n** running in service: pdfgen **\n"
	$(eval SERVICE=$(PDFGEN_SERVICE))

# Helpers
run-docker-cmd:
	@if [ "$(SERVICE)" = "" ]; then \
		docker-compose -f $(DC_FILE) $(DC_ACTION) $(ARGS); \
	else \
		docker-compose -f $(DC_FILE) $(DC_ACTION) $(SERVICE) $(ARGS); \
	fi; \

run-cmd:
	@if [ "$(SERVICE)" = "" ]; then \
		cd $(PWD)/api && $(ARGS); \
		cd $(PWD)/web && $(ARGS); \
		cd $(PWD)/pdfgen && $(ARGS); \
	else \
		cd $(PWD)/$(SERVICE) && $(ARGS); \
	fi; \


# Actions
## - actions that will affect containers
serve:
	make SERVICE=$(SERVICE) DC_ACTION=up run-docker-cmd

build-image:
	make SERVICE=$(SERVICE) DC_ACTION=build run-docker-cmd

test:
	@if [ "$(SERVICE)" = "" ]; then \
		echo "\ntest action needs a target!\n"; \
	else \
		make SERVICE=$(SERVICE) DC_ACTION="run --rm" ARGS="yarn test" run-docker-cmd; \
	fi

test-watch:
	@if [ "$(SERVICE)" = "" ]; then \
		echo "\ntest-watch action needs a target!\n"; \
	else \
		make SERVICE=$(SERVICE) DC_ACTION="run --rm" ARGS="yarn test --watchAll" run-docker-cmd; \
	fi

connect:
	make SERVICE=$(SERVICE) DC_ACTION="run --rm" ARGS="/bin/sh" run-docker-cmd

deploy:
	@case "$(SERVICE)" in \
	web) \
		docker-compose -f docker-compose.production.yml up --build web \
		;; \
	api) \
		cd $(PWD)/api; \
		gcloud builds submit --tag gcr.io/tech-resume/api \
		;; \
	*) \
		echo "cannot run deploy cmd on service: $(SERVICE)"; \
		exit 1 \
		;; \
	esac

deploy-base-image:
	gcloud builds submit --tag gcr.io/tech-resume/bs-node

## - actions that will affect your local filesystem
clean:
	make SERVICE=$(SERVICE) ARGS="yarn clean" run-cmd

install:
	@npm install -g bs-platform@7.0.1; \
	make SERVICE=$(SERVICE) ARGS="yarn install" run-cmd; \
	make SERVICE=$(SERVICE) ARGS="npm link bs-platform" run-cmd;

.PHONY: web api pdfgen start build-image clean install test connect
