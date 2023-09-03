#-march=native

main:
	gcc main.c -o main -O3 -Wall -mcpu=apple-m1 -std=c11

.PHONY: clean
clean:
	@echo --------------------------------------------------
	@echo Removing main
	rm main

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

