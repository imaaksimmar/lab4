.PHONY: all tests clean ui

CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread -I.

GTEST_DIR = googletest/googletest
GTEST_CPPFLAGS = -isystem $(GTEST_DIR)/include -I$(GTEST_DIR)

TEST_PROGRAM = run_tests
TEST_SRCS = $(wildcard tests/*.cpp)
GTEST_SRCS = $(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc

UI_DIR = ui/build
UI_APP = ui/build/StatisticsUI.app/Contents/MacOS/StatisticsUI
QT_PATH = /opt/homebrew/Cellar/qt/6.11.0

all: $(TEST_PROGRAM)

$(TEST_PROGRAM): $(TEST_SRCS) $(GTEST_SRCS)
	$(CXX) $(CXXFLAGS) $(GTEST_CPPFLAGS) $^ -o $@

tests: $(TEST_PROGRAM)
	./$(TEST_PROGRAM)

ui:
	@mkdir -p $(UI_DIR)
	@cd $(UI_DIR) && cmake .. -DCMAKE_PREFIX_PATH=$(QT_PATH) \
		-DCMAKE_CXX_STANDARD=20 -Wno-dev > /dev/null 2>&1
	@cd $(UI_DIR) && make -s
	@$(UI_APP)

clean:
	rm -f $(TEST_PROGRAM)
	rm -rf $(UI_DIR)