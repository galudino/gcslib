###
 #	@file		Makefile 
 #	@brief		Makefile for struct vector_ptr ADT and tests
 #
 #	@author		Gemuele (Gem) Aludino
 #	@date		04 Jan 2020
 #	@copyright	Copyright © 2020 Gemuele Aludino
 ## 
###
 #	Copyright © 2020 Gemuele Aludino
 #
 # 	Permission is hereby granted, free of charge, to any person obtaining
 # 	a copy of this software and associated documentation files (the "Software"),
 # 	to deal in the Software without restriction, including without limitation
 # 	the rights to use, copy, modify, merge, publish, distribute, sublicense,
 # 	and/or sell copies of the Software, and to permit persons to whom the
 # 	Software is furnished to do so, subject to the following conditions:
 #
 # 	The above copyright notice and this permission notice shall be included
 # 	in all copies or substantial portions of the Software.
 #
 # 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 # 	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 # 	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 # 	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 # 	DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 # 	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 # 	THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ##

DIR_INCLUDE	=	./

SRC_TEST	=	test.c
SRC_VECTOR	=	vector_ptr.c

OBJ_VECTOR	=	vector_ptr.o

INC_VECTOR	=	vector_ptr.h

EXE_TEST	=	test

CC		=	cc
CSTD		=	-std=c89
PEDANTIC	=	-pedantic-errors
WALL		=	-Wall
WERR		=	-Werror
DEBUG		=	-g
OPTIMIZED	=	-O3

CFLAGS		=	$(CSTD) $(PEDANTIC) $(WALL) $(WERR) $(DEBUG) $(OPTIMIZED)

all:	$(EXE_TEST)

$(EXE_TEST):	$(SRC_TEST) $(OBJ_VECTOR) $(INC_VECTOR)
	$(CC) -o $(EXE_TEST) $(SRC_TEST) $(OBJ_VECTOR) $(CFLAGS) -I $(DIR_INCLUDE)

$(OBJ_VECTOR): $(SRC_VECTOR) $(INC_VECTOR)
	$(CC) -c $(SRC_VECTOR) -o $(OBJ_VECTOR) $(CFLAGS) -I $(DIR_INCLUDE)

clean:
	rm -rf *.o $(EXE_TEST) *.dSYM || true

