
CXX = g++
CXXFLAGS = -std=c++20 -Wall -pthread -Iinclude
TARGET = build/App
SRCS = src/app.cpp
TEST_TARGET = tests/test
TEST_SRC = tests/main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

tests: $(TEST_TARGET)
	
$(TEST_TARGET): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SRC)

clean:
	rm -f $(TARGET)
