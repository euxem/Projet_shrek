CC=gcc
CFLAG=-Wall -Werror -Wextra -Wpedantic -std=gnu99 -g

all:shrek test_lexeme test_syntax test_ast clean

# Links

lecture_caracteres.o:lecture_caracteres.c lecture_caracteres.h
	$(CC) $(CFLAG) -c $<

analyse_lexicale.o:analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h
	$(CC) $(CFLAG) -c $<

analyse_syntaxique.o:analyse_syntaxique.c analyse_syntaxique.h analyse_lexicale.h lecture_caracteres.h type_ast.h
	$(CC) $(CFLAG) -c $<

type_ast.o:type_ast.c type_ast.h
	$(CC) $(CFLAG) -c $<

table_symbole.o:table_symbole.c table_symbole.h analyse_lexicale.h lecture_caracteres.h type_ast.h
	$(CC) $(CFLAG) -c $<

arbre_ast.o:arbre_ast.c arbre_ast.h type_ast.h table_symbole.h analyse_lexicale.h
	$(CC) $(CFLAG) -c $<

main_syntax_test.o: main_syntax_test.c analyse_syntaxique.h analyse_lexicale.h lecture_caracteres.h type_ast.h
	$(CC) $(CFLAG) -c $<

main_lexicale_test.o:main_lexicale_test.c analyse_lexicale.h lecture_caracteres.h
	$(CC) $(CFLAG) -c $<

main_ast_test.o:main_ast_test.c analyse_lexicale.h lecture_caracteres.h analyse_syntaxique.h type_ast.h table_symbole.h arbre_ast.h
	$(CC) $(CFLAG) -c $<

shrek.o:shrek.c analyse_lexicale.h lecture_caracteres.h analyse_syntaxique.h type_ast.h table_symbole.h arbre_ast.h
	$(CC) $(CFLAG) -c $<

# Compile
shrek:shrek.o analyse_lexicale.o lecture_caracteres.o analyse_syntaxique.o type_ast.o table_symbole.o arbre_ast.o
	$(CC) $(CFLAG) $^ -o $@
	make clean

test_ast:main_ast_test.o analyse_lexicale.o lecture_caracteres.o analyse_syntaxique.o type_ast.o table_symbole.o arbre_ast.o
	$(CC) $(CFLAG) $^ -o $@

test_syntax:main_syntax_test.o analyse_lexicale.o lecture_caracteres.o analyse_syntaxique.o type_ast.o
	$(CC) $(CFLAG) $^ -o $@

test_lexeme:main_lexicale_test.o analyse_lexicale.o lecture_caracteres.o
	$(CC) $(CFLAG) $^ -o $@

clean:
	rm -f *.o

clear:clean
	rm -f test_lexeme test_syntax test_ast