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
CXX=g++
# CXXFLAGS=-g -std=c++11 -fext-numeric-literals -Wall -pedantic
CXXFLAGS=-g -std=c++14 -fext-numeric-literals -Wall -Wno-comment -pedantic -O2
# CXXFLAGS=-g -std=c++11 -fext-numeric-literals -Wall -Wno-comment -pedantic -O3
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

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
