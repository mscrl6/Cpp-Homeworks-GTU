all: clean compile run

compile: main.cpp shell.cpp shell.h directory.cpp directory.h file.h file.cpp
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ -o shell main.cpp shell.cpp directory.cpp file.cpp

run:
	@echo "-------------------------------------------"
	@echo "Running the tests...."
	@echo "======================================================================="
	@./shell
	@echo "======================================================================="
	@echo "Completed tests...."

clean:
	@echo "-------------------------------------------"
	@echo "Cleaning..."
	@rm -f *.o
	@rm -f shell
