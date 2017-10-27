#ifndef ARQUIVO
#include <iostream>
using namespace std;

typedef struct{
    char nome[49];
    char cpf[11]
    int idade;
} Registro;

typedef struct{
    int topo;
} RegistroCabecalho;

const TAM_REG = sizeof(Registro);
const TAM_REG_CABECALHO = sizeof(RegistroCabecalho)

class Arquivo{
    private:
        FILE * arquivo;
        RegistroCabecalho rcabecalho;
    public:
        Arquivo::Arquivo(string nomeArquivo)

}

Arquivo::Arquivo(string nomeArquivo, char operacao){
    this->arquivo = fopen(nomeArquivo, "wb");
    fwrite(&this->rcabecalho, TAM_REG_CABECALHO, 1, this->arquivo);
    fclose(this->arquivo)
}