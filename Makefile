CC = g++
CFLAGS = -O2
TARGET = main
SRCS = main.cpp heuristics.cpp manipulacao_arquivo.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = f_heuristics.h funcoes.h

$(TARGET): main.cpp heuristics.cpp manipulacao_arquivo.cpp
	$(CC) $(CFLAGS) main.cpp heuristics.cpp manipulacao_arquivo.cpp -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
