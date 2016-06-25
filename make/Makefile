AR := ar crs
ROOT=../../..
WORK_ROOT=..

CXX=g++
CXXFLAGS=-pipe -Wall -W -fPIC -O
#INCLUDE=-I$(ROOT)/public/log/include -I$(WORK_ROOT)/include
INCLUDE=-I$(ROOT)/third/boost/1.61.0/include \
		-I$(ROOT)/third/gtest/1.7.0/include \
		-I$(ROOT)/third/glog/0.3.4/include \
		-I$(WORK_ROOT)/include

LIBRARY=-L$(ROOT)/third/boost/1.61.0/lib \
		-L$(ROOT)/third/gtest/1.7.0/lib \
		-L$(ROOT)/third/glog/0.3.4/lib

SOURCE_DIR=$(WORK_ROOT)/src
HEADER_DIR=$(WORK_ROOT)/include
UNITTEST_DIR=$(WORK_ROOT)/unittest/case
UNITTEST_DEST_DIR=$(WORK_ROOT)/unittest/bin
OUTPUT_DIR=$(WORK_ROOT)/make/output
OBJECT_DIR=$(OUTPUT_DIR)/obj
TARGET_DIR=$(OUTPUT_DIR)/lib
UT_DIR=$(OUTPUT_DIR)/ut

SOURCE=$(foreach dir, $(subst :, ,$(SOURCE_DIR)), $(wildcard $(dir)/*.cc))
OBJECT=$(addprefix $(OBJECT_DIR)/, $(foreach obj,$(subst .cc,.o,$(SOURCE)),$(shell basename $(obj))))
TARGET=$(TARGET_DIR)/libconfigure.a
UNITTEST_SRC=$(foreach dir, $(subst :,  ,$(UNITTEST_DIR)), $(wildcard $(dir)/*.cc))
UNITTEST_DEST=$(subst case,bin, $(subst .cc,  ,$(UNITTEST_SRC)))

MAKE_DIR := $(shell mkdir -p $(OBJECT_DIR) $(TARGET_DIR) $(UNITTEST_DEST_DIR))

all: $(TARGET) $(UNITTEST_DEST)
	@echo "\033[1;5;32mBuild $^ succeed\033[0m"

$(OBJECT): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cc $(HEADER_DIR)/%.h $(HEADER_DIR)/Constants.h
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCLUDE)

$(TARGET): $(OBJECT)
	$(AR) $@ $^ 

$(UNITTEST_DEST): $(UNITTEST_DEST_DIR)/%: $(UNITTEST_DIR)/%.cc $(OBJECT)
	$(CXX) $^ -o $@ $(INCLUDE) $(LIBRARY) -lgtest -lglog

publish: $(TARGET)
	rm -rf $(WORK_ROOT)/lib
	cp -r $(TARGET_DIR) $(WORK_ROOT)
	@echo "\033[1;5;32mPublish succeed!\033[0m"

clean:
	@rm -rf $(OUTPUT_DIR)

clean-all:
	@rm -rf $(OUTPUT_DIR)
	@rm -rf $(WORK_ROOT)/lib
	@rm -rf $(UNITTEST_DEST_DIR)/*

.PHONY: all clean clean-all publish
