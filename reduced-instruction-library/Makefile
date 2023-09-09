#-march=native

main:
	gcc main.c -o main -O3 -Wall -mcpu=apple-m1 -std=c11 -I "./include" -fmacro-backtrace-limit=0

.PHONY: clean
clean:
	@echo --------------------------------------------------
	@echo Removing main
	rm main

.PHONY: run
run: 
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

.PHONY: test
test:
	gcc test.c -o test -O3 -Wall -mcpu=apple-m1 -std=c11 -I "./include" -fmacro-backtrace-limit=0 
	@echo --------------------------------------------------
	@echo Running test
	./test
	@echo --------------------------------------------------
	rm test
