#ifndef ARQUIVOINDICE
#include <iostream>
#include <cstring>
using namespace std;

typedef struct{
    char chave[12];
    int rrn;
} RegistroIndice;

const int TAM_REG_INDICE = sizeof(RegistroIndice);

class ArquivoIndice{
	private:
		const string nome;
    	FILE * arquivo;
    public:
    	ArquivoIndice(string nome);
    	void insere(const char *, const int &);
    	int busca(const char *);
};

ArquivoIndice::ArquivoIndice(string nomeArquivo): nome(nomeArquivo + "-indice"){
	this->arquivo = fopen((nomeArquivo + "-indice").c_str(), "wb");
	fclose(this->arquivo);
};

void ArquivoIndice::insere(const char * cpf, const int &  posicao){
	RegistroIndice * rAux = new RegistroIndice;// criando um novo registro de índice
	// preenchendo o novo registro de índice
	sprintf(rAux->chave, "%s", cpf);
	fflush(stdin);
	rAux->rrn = posicao;
	// adicionando no arquivo de índice
	arquivo = fopen(nome.c_str(), "ab"); // append
	fwrite(rAux, TAM_REG_INDICE, 1, arquivo);
	fclose(arquivo);
	delete rAux;
};

int ArquivoIndice::busca(const char * cpf){
	RegistroIndice rAux;

	arquivo = fopen(nome.c_str(), "rb");
	while(1 == fread(&rAux, TAM_REG_INDICE, 1, arquivo)) // busque sequencialmente no indice
		if(!strcmp(rAux.chave, cpf)) // se forem iguais (achou a chave)
			return rAux.rrn; // retorne o rrn da chave
	
	return -1; // senao, retorne -1
};

#endif