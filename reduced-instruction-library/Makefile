#-march=native

main:
	gcc main.c -o main -O0 -Wall -mcpu=apple-m1 -std=c11 -I "./include"

.PHONY: clean
clean:
	@echo --------------------------------------------------
	@echo Removing main
	rm main

.PHONY: test
test:
	gcc test.c -o test -I "include"
	@echo --------------------------------------------------
	@echo Running test
	./test

.PHONY: run
run: 
	make main
	make clean
	make main
	@echo --------------------------------------------------
	@echo Running main
	./main
	@echo --------------------------------------------------

.PHONY: git
git:
	git add .
	git commit -m .
	git push

