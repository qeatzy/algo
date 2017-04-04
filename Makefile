# a dev-branch is to be merged here is ./sample.mk
# http://make.mad-scientist.net/papers/multi-architecture-builds/
# used in conjuction with target.mk
ifeq (,$(filter _%,$(notdir $(CURDIR))))
include target.mk
else

RM       = rm -f
#----- End Boilerplate

VPATH = $(SRCDIR)

# Normal makefile rules here
# "cpp
CXX     = g++
DEBUG   = -g
# DEBUG   = -g -H
# CXXFLAGS  = -Wall $(DEBUG) -std=c++11 -pthread
# LDFLAGS = -pthread
# CXXFLAGS=$(DEBUG) -std=c++14 -fext-numeric-literals -Wall -Wno-comment -pedantic -O2
CXXFLAGS=$(DEBUG) -std=c++14 -fext-numeric-literals -Wno-comment -pedantic -O2

OBJS = 
.PHONY: all utils
all: utils
%: %.o
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)
%.o: %.cpp utility.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<
utils: utility
utility: utility.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

# %: %.cpp
# 	$(CXX) $(CXXFLAGS) $< -o $@

# "java
CLASSPATH=_test
JCC=javac
# JFLAGS=-g -d ../_test
JFLAGS=-g -d .
.java.class:
	$(JCC) $(JFLAGS) ../$*.java
.SUFFIXES: .java .class   # https://www.cs.swarthmore.edu/~newhall/unixhelp/javamakefiles.html
CLASSES=*.java
classes: $(CLASSES:.java=.class)

#----- Begin Boilerplate
endif
