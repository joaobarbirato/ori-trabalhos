#ifndef ARQUIVO
#include <iostream>
#include "ArquivoIndice.hpp"

using namespace std;

typedef struct{
    char nome[48];
    char cpf[12];
    int idade;
} Registro;

typedef struct{
    int topo;
} RegistroCabecalho;

const int TAM_REG = sizeof(Registro);
const int TAM_REG_CABECALHO = sizeof(RegistroCabecalho);

class Arquivo{
    private:
        const string nome;
        FILE * dados;
        ArquivoIndice * indice;
        RegistroCabecalho rcabecalho;
    public:
        Arquivo(string nomeArquivo);
        ~Arquivo();
        int getTopo();
        void insere(const Registro *);
        void lista();
};

Arquivo::Arquivo(string nomeArquivo): nome(nomeArquivo){
    this->dados = fopen(nomeArquivo.c_str(), "wb");
    indice = new ArquivoIndice(nomeArquivo);
    this->rcabecalho.topo = -1;
    fwrite(&this->rcabecalho, TAM_REG_CABECALHO, 1, this->dados);
    fclose(this->dados);
};

Arquivo::~Arquivo(){
    delete indice;
};

int Arquivo::getTopo(){ return rcabecalho.topo; };

void Arquivo::insere(const Registro * reg){
    char arroba;
    int rrn;
    dados = fopen(nome.c_str(), "r+b");
    // verificar se há espaços vazios
    if(rcabecalho.topo == -1){ // se não há espaços vazios
        fseek(dados, 0, SEEK_END);
        fwrite(reg, TAM_REG, 1, dados);
    }else{ // se há
        fseek(dados, TAM_REG * rcabecalho.topo + TAM_REG_CABECALHO + sizeof('@'), SEEK_SET); // vá ao topo da lista
        fread(&rrn, sizeof(int), 1, dados); // guarde o próximo da lista (novo topo)
        rcabecalho.topo = rrn;
        fseek(dados, -(sizeof('@')+sizeof(int)), SEEK_CUR); // volte o ponteiro
        fwrite(reg, TAM_REG, 1, dados); // escreva o registro
    }
    fclose(dados);
};

void Arquivo::lista(){

};
    

#endif