# compilador:
# gcc versão 7
CC=g++

# arquivo fonte (classe)
fonte=src/Arquivo.hpp

# arquivo main
main=script/main.cpp

# executavel
programa=./bin/executavel

all:
	$(programa)

$programa: $fonte

	$(CC) $(fonte) $(main) -o $(programa)

clean:
	rm -f $(programa)