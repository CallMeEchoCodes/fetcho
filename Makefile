EXEC = quickfetch

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj

SRC_DIR = src
SRCS := $(sort $(shell find $(SRC_DIR) -name '*.cpp'))

CXX = g++
CXXFLAGS = -Wall -std=c++17
CPPFLAGS = 
LDFLAGS = 

LDLIBS = -lprocps

ifeq ($(RELEASE),1)
	BUILD_DIR := $(BUILD_DIR)/release
	RELEASE := 1
	CXXFLAGS += -O2
	CXXFLAGS += -s
else
	BUILD_DIR := $(BUILD_DIR)/debug
	RELEASE := 0
	CXXFLAGS += -Og
	CXXFLAGS += -g
endif

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: clean build install

build: buildtext $(BIN_DIR)/$(EXEC) 
	@if [ "$(RELEASE)" = "1" ]; then echo -e "\e[95m==> \e[0;1mStripping executable $(EXEC)…\e[0m"; strip --strip-unneeded $(BIN_DIR)/$(EXEC); fi

$(BIN_DIR)/$(EXEC): $(OBJS)
	@echo -e "\e[93m==> \e[0;1mLinking executable $(EXEC)…\e[0m"
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo -e "\e[92m==> \e[0;1mCompiling $<…\e[0m"
	@mkdir -p $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

buildtext:
	@echo -e "\e[1;91m> \e[0;1mBuilding $(EXEC)…\e[0m"

run: build
	@echo -e "\e[1;94m> \e[0;1mRunning $(EXEC)…\e[0m"
	@$(BIN_DIR)/$(EXEC)

clean:
	@echo -e "\e[1;96m> \e[0;1mCleaning…\e[0m"
	@rm -rf $(BUILD_DIR)

install: build
	@if [ "$(RELEASE)" != "1" ]; then echo -e "\e[1;93m> Installing requires you to be in release mode!\e[0m"; exit 1; fi
	@echo -e "\e[1;93m> \e[0;1mInstalling $(EXEC)…\e[0m"
	@sudo install -sDpg0 -o0 -m755 $(BIN_DIR)/$(EXEC) /usr/local/bin/

uninstall:
	@echo -e "\e[1;93m> \e[0;1mUninstalling $(EXEC)…\e[0m"
	@sudo rm /usr/local/bin/$(EXEC)
