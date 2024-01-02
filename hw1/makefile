all: clean compile run

compile: main.cpp ChessBoard.cpp ChessBoard.h Pieces.cpp Pieces.h
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ -o chessGame main.cpp ChessBoard.cpp Pieces.cpp

run:
	@echo "-------------------------------------------"
	@echo "Running the tests...."
	@echo "======================================================================="
	@./chessGame
	@echo "======================================================================="
	@echo "Completed tests...."

clean:
	@echo "-------------------------------------------"
	@echo "Cleaning..."
	@rm -f *.o
	@rm -f chessGame
