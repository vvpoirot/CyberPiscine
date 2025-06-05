all: build

build:
	@docker-compose up -d --build

clear:
	@docker rm -f $$(docker ps -aq) || true
	@docker rmi -f $$(docker images -q) || true

list:
	@docker ps -a

re: clear build

.PHONY: all build clear list