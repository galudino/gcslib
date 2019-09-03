##
##	@file		Makefile
##	@brief		Makefile for gcslib
##
##	@author		Gemuele Aludino
##	@date      	18 Jul 2019
##	@copyright 	Copyright © 2019 Gemuele Aludino. All rights reserved.
##

## DIRECTORIES ################################################################
DIR_OBJ				= build
DIR_INC				= include
DIR_SRC				= src
DIR_TST				= test
DIR_CLI				= client
###############################################################################

## EXECUTABLE SOURCE FILE NAMES ###############################################
SRC_CLI				= gcslib.c
###############################################################################

## EXECUTABLE NAMES ###########################################################
EXE_CLI 			= gcslib
###############################################################################

## COMPILER ###################################################################
CC 				= gcc
###############################################################################

## FLAGS ######################################################################

## Flag types - right-hand side can be modified or commented out to disable ##
CSTD 				= -std=gnu99
DEBUG 				= -g
PTHREAD				= -pthread
MATH 				= -lm      ## disable for macOS/clang

SANTIIZED 			= #-fsanitize=address
W_ALL 				= #-Wall
W_ERR 				= #-Werror

## Add new flag categories here ##############################################
CFLAGS 				= $(DEBUG) $(CSTD) $(W_ALL) $(W_ERR) $(SANITIZED)
LIB 				= $(PTHREAD) $(MATH)
INC 				= -I $(DIR_INC)
###############################################################################

## SOURCES/ASSEMBLY/OBJECTS/EXECUTABLES #######################################
EXT_SRC := .c
EXT_INC	:= .h
EXT_OBJ := .o

## Used for directives below
SOURCES	:= $(wildcard $(DIR_SRC)/*$(EXT_SRC))
OBJECTS := $(patsubst $(DIR_SRC)/%$(EXT_SRC), $(DIR_OBJ)/%$(EXT_OBJ), $(SOURCES))

ALL_EXE	= $(EXE_CLI)
###############################################################################

## DIRECTIVES #################################################################

##
## $(CC) flags
##
## Preprocessing	-E
##	preprocessor directives in the input source code are expanded;
## 	comments are omitted
##
## Compilation		-S
##	post preprocessing, input source code is compiled (translated)
##	to the target system architecture's assembly language
##
## Assembly			-c
## 	post compilation, assembly language code is translated to object code,
## 	which consists of instructions to be executed by the target system architecture
##	(although the resulting object files are of binary format, these files are not
##	 executable programs)
##
## Linkage			-o
##	post assembly, object code contained within the object files are linked with
##	other object code in other object files to form the final binary executable;
##	the result is a program that will run on the target system architecture
##
## This Makefile includes instructions for producing object code from
## plaintext source code, as well as instructions for producing executables from
## object files -- the intermediary steps have been skipped for brevity.
##

## Source code is preprocessed, compiled, and assembled - .o object files produced
$(DIR_OBJ)/%$(EXT_OBJ): $(DIR_SRC)/%$(EXT_SRC)
	$(CC) -c $< -o $@ $(CFLAGS) $(INC)

## Links .o object files - binary executable produced
$(EXE_CLI): $(OBJECTS)
	@echo;
	@echo "Linking $(EXE_CLI)..."
	@echo;

	$(CC) -o $(EXE_CLI) $(DIR_CLI)/$(SRC_CLI) $(OBJECTS) $(CFLAGS) $(LIB) $(INC)

	@echo;
	@echo "Linking complete."
	@echo;

all:	$(ALL_EXE)

gcslib: $(OBJECTS)

clean:
	@echo;
	@echo "Cleaning..."
	@echo;

	@echo;
	@echo "Removing object files..."
	@echo;

	rm $(OBJECTS)

	@echo;
	@echo "Removed all object files."
	@echo;

	@echo;
	@echo "Removing executables..."
	@echo;

	rm $(ALL_EXE)

	@echo;
	@echo "Removed all executables."
	@echo;

	@echo;
	@echo "Removing packages..."
	@echo;

	rm -r *.dSYM

	@echo;
	@echo "Packages removed."
	@echo;

	@echo;
	@echo "Clean complete."
	@echo;

###############################################################################
