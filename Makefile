BIN:=template_5
all:
	g++ -std=c++11 -o ./bin/$(BIN) -O0 -g $(BIN).cpp
run:
	./bin/$(BIN)
