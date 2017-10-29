#ifndef ARQUIVOINDICE
#include <iostream>
using namespace std;

typedef struct{
    char cpf[11];
    int rrn;
} registroIndice;

const int TAM_REG_INDICE = 15;

class ArquivoIndice{
	private:
    	FILE * arqIndice;	
    public:
    	ArquivoIndice(string nome);
};

ArquivoIndice::ArquivoIndice(string nome){
	this->arqIndice = fopen((nome + "-indice").c_str(), "wb");
	fclose(this->arqIndice);
};

#endif