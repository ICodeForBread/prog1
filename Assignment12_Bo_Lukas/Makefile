CC = gcc
LINKER = gcc
CFLAGS = -std=c99 -Wall # -Werror -Wpointer-arith -Wfatal-errors
DEBUG = -g
PROG1LIBNAME = prog1
PROG1LIBDIR = ../prog1lib/lib

BOOKS = book_list
SRCS = book_list.c pointer_list.c
OBJS = $(SRCS:.c=.o)

EXERCISE = shopping_cart
SRCS_EXERCISE = shopping_cart.c pointer_list.c
OBJS_EXERCISE = $(SRCS_EXERCISE:.c=.o)

SOLUTION = shopping_cart_solution
SRCS_SOLUTION = shopping_cart_solution.c pointer_list.c
OBJS_SOLUTION = $(SRCS_SOLUTION:.c=.o)

EXT = pointer_list_ext
SRCS_EXT = pointer_list_ext.c pointer_list.c
OBJS_EXT = $(SRCS_EXT:.c=.o)

EXT_SOLUTION = pointer_list_ext_solution
SRCS_EXT_SOLUTION = pointer_list_ext_solution.c pointer_list.c
OBJS_EXT_SOLUTION = $(SRCS_EXT_SOLUTION:.c=.o)

# disable default suffixes
.SUFFIXES:

# library
prog1lib:
	cd $(PROG1LIBDIR) && make

# pointer list
pointer_list.o: pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR) 

pointer_list: pointer_list.o
	$(LINKER) $(CFLAGS) $(DEBUG) -o pointer_list pointer_list.o -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR)

# book_list
book_list.o: book_list.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR)

$(BOOKS): $(OBJS)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(BOOKS) $(OBJS) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR)

# exercise
shopping_cart.o: shopping_cart.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR)

$(EXERCISE): $(OBJS_EXERCISE)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(EXERCISE) $(OBJS_EXERCISE) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR)

# solution

shopping_cart_solution.o: shopping_cart_solution.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR)

$(SOLUTION): $(OBJS_SOLUTION)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(SOLUTION) $(OBJS_SOLUTION) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR)

# pointer_list_ext
pointer_list_ext.o: pointer_list_ext.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR) 

$(EXT): $(OBJS_EXT)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(EXT) $(OBJS_EXT) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR) 

# pointer_list_ext_solution
pointer_list_ext_solution.o: pointer_list_ext_solution.c pointer_list.c pointer_list.h
	$(CC) -c $(CFLAGS) $(DEBUG) -o $@ $< -iquote$(PROG1LIBDIR) 

$(EXT_SOLUTION): $(OBJS_EXT_SOLUTION)
	$(LINKER) $(CFLAGS) $(DEBUG) -o $(EXT_SOLUTION) $(OBJS_EXT_SOLUTION) -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -lm -iquote$(PROG1LIBDIR) 

# generic pattern rule for compiling .c-file to executable
%: %.c prog1lib
	$(CC) $(CFLAGS) $(DEBUG) $< -L$(PROG1LIBDIR) -l$(PROG1LIBNAME) -iquote$(PROG1LIBDIR) -o $@ $(MATH)

# delete intermediary files
.PHONY: clean		# do not treat "clean" as a file name
clean:				# special target to remove files: make clean
	rm -f $(BOOKS) $(OBJS) $(EXERCISE) $(OBJS_EXERCISE) $(OBJS_EXT)
