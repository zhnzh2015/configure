all: test

CXX=g++
WORK_ROOT=../../../..
LOCAL_ROOT=../..
INCLUDE=-I$(WORK_ROOT)/public/log/include \
		-I$(WORK_ROOT)/public/configure/include \
		-I$(WORK_ROOT)/third/gtest/1.7.0/include

LIBRARY=-L$(WORK_ROOT)/public/log/lib \
		-L$(WORK_ROOT)/public/configure/lib \
		-L$(WORK_ROOT)/third/gtest/1.7.0/lib

test.o : $(LOCAL_ROOT)/unittest/case/test.cc
	$(CXX) -o $@ -c $< $(INCLUDE)

test : test.o
#	$(CXX) $^ -o $@ $(INCLUDE) $(LIBRARY) -lbm_log -lbm_configure -lgtest_main
	$(CXX) $^ -o $@ $(INCLUDE) $(LIBRARY) -lbm_log -lbm_configure

clean: 
	rm -f *.o
	rm -f test

