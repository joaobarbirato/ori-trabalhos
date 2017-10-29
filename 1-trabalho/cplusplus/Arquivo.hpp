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
        int ultimoRRN;
    public:
        Arquivo(string nomeArquivo);
        ~Arquivo();
        int getTopo();
        void insere(const Registro &);
        void lista();
        bool busca(const char *, Registro &);
};

Arquivo::Arquivo(string nomeArquivo): nome(nomeArquivo){
    this->dados = fopen(nomeArquivo.c_str(), "wb");
    indice = new ArquivoIndice(nomeArquivo);
    this->rcabecalho.topo = -1;
    fwrite(&this->rcabecalho, TAM_REG_CABECALHO, 1, this->dados);
    fclose(this->dados);

    this->ultimoRRN = 0;
};

Arquivo::~Arquivo(){
    delete indice;
};

int Arquivo::getTopo(){ return rcabecalho.topo; };

void Arquivo::insere(const Registro & reg){
    int rrn;
    dados = fopen(nome.c_str(), "r+b");
    // verificar se há espaços vazios
    if(rcabecalho.topo == -1){ // se não há espaços vazios
        fseek(dados, 0, SEEK_END);
        fwrite(&reg, TAM_REG, 1, dados);
        indice->insere(reg.cpf, ultimoRRN);
        ultimoRRN++;
    }else{ // se há
        fseek(dados, TAM_REG * rcabecalho.topo + TAM_REG_CABECALHO + sizeof('@'), SEEK_SET); // vá ao topo da lista
        fread(&rrn, sizeof(int), 1, dados); // guarde o próximo da lista (novo topo)
        rcabecalho.topo = rrn;
        fseek(dados, -(sizeof('@')+sizeof(int)), SEEK_CUR); // volte o ponteiro
        fwrite(&reg, TAM_REG, 1, dados); // escreva o registro
    }
    fclose(dados);
};

void Arquivo::lista(){
    Registro rAux;
    int k;
    char ehArroba;

    dados = fopen(nome.c_str(), "rb");
    fseek(dados, TAM_REG_CABECALHO, SEEK_SET);
    // exibição sequencial dos dadoss
    while(1 == fread(&rAux, TAM_REG, 1, dados)){ // se não chegou no fim do arquivo binário
        // verificar se o registro lido está removido
        fread(&ehArroba, sizeof(char),1,dados);
        fseek(dados, -sizeof(char), SEEK_CUR);
        if(ehArroba != '@'){ // se nao achar '@', não está removido
            cout << endl;
            cout << rAux.nome << endl;
            cout << rAux.cpf << endl;
            cout << rAux.idade << endl;
            cout << endl;
        }else{ // se está removido
            cout << endl << "REMOVIDO" << endl << endl << endl << endl;
        }
    }
    fclose(dados);
};

bool Arquivo::busca(const char * chave, Registro & R){
    //TODO: Tratar string com tamanho menor
    int posicao = indice->busca(chave);
    if(posicao == -1) // se nao achou
        return false; 
    else{ // se achou
        dados = fopen(nome.c_str(), "rb");
        fseek(dados, TAM_REG_CABECALHO + TAM_REG * posicao, SEEK_SET); // vá até a posicao
        fread(&R, TAM_REG, 1, dados); // leia o registro e passe-o por referência
        fclose(dados);
    }
    return true;
};


#endif