.PHONY: start

start:
	cd api; \
	yarn concurrently \
		--names bsb,node \
		--prefix-colors blue.bold,yellow.bold \
		--kill-others \
		--kill-others-on-fail \
			"yarn watch-bsb" "yarn watch-node"; \
	cd ..;
