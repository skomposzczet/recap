TARGET = recap.so

CXX = g++
CXXFLAGS += -std=c++20 -Wall -Wextra -Ihdr -fPIC -g
LFLAGS += -shared

DEP_FLAGS += -MMD -MP
CXXFLAGS+=$(DEP_FLAGS)

SRCDIR = src
OBJDIR = obj
HDRDIR = hdr

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
HEADERS := $(wildcard $(HDRDIR)/*.hpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPENDS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.d)

LIBPREFIX = /usr/local
LIBDIR = $(LIBPREFIX)/lib

GTEST_DIR = googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

CPPTESTFLAGS += -isystem $(GTEST_DIR)/include
CXXTESTFLAGS += -std=c++20 -g -Wall -Wextra -pthread -Ihdr

TEST_SRCDIR = tests/src
TEST_OBJDIR = tests/obj

TEST_SOURCES := $(wildcard $(TEST_SRCDIR)/*.cpp)
TEST_OBJECTS := $(TEST_SOURCES:$(TEST_SRCDIR)/%.cpp=$(TEST_OBJDIR)/%.o)

TESTS = recap_ut.test

all: $(OBJDIR)/$(TARGET)

$(OBJDIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean install test


test: $(OBJDIR)/$(TARGET) $(TEST_OBJDIR)/$(TESTS) 
	@./$(TEST_OBJDIR)/$(TESTS)

$(TEST_OBJDIR)/$(TESTS): $(TEST_OBJECTS) $(OBJECTS) $(TEST_OBJDIR)/gtest.a $(TEST_OBJDIR)/gtest_main.a
	$(CXX) $(CPPTESTFLAGS) $(CXXTESTFLAGS) $^ -o $@

$(TEST_OBJECTS): $(TEST_OBJDIR)/%.o : $(TEST_SRCDIR)/%.cpp
	@mkdir -p $(TEST_OBJDIR)
	$(CXX) $(CPPTESTFLAGS) $(CXXTESTFLAGS) -c $< -o $@

$(TEST_OBJDIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPTESTFLAGS) -I$(GTEST_DIR) $(CXXTESTFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(TEST_OBJDIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPTESTFLAGS) -I$(GTEST_DIR) $(CXXTESTFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(TEST_OBJDIR)/gtest.a : $(TEST_OBJDIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TEST_OBJDIR)/gtest_main.a : $(TEST_OBJDIR)/gtest-all.o $(TEST_OBJDIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


clean:
	@rm -rf $(OBJDIR)/ $(TEST_OBJDIR)

install:
	@cp $(OBJDIR)/$(TARGET) $(LIBDIR)/lib$(TARGET)
	@echo "Installed"

-include $(DEPENDS)