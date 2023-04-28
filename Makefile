TARGET = recap.so

CXX = g++
CXXFLAGS += -std=c++23 -Wall -Wextra -Ihdr -fPIC -g
LFLAGS += -shared -lbfd

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

all: $(OBJDIR)/$(TARGET)

$(OBJDIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean install

clean:
	@rm -rf $(OBJDIR)/

install:
	@cp $(OBJDIR)/$(TARGET) $(LIBDIR)/lib$(TARGET)
	@echo "Installed"

-include $(DEPENDS)