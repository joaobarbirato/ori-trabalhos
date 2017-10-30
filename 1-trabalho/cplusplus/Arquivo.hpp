#ifndef ARQUIVO
#include <iostream>
#include <cstring>

using namespace std;

typedef struct{
    char cpf[12];
    char nome[51];
    int idade;
} Registro;

typedef struct{
    int topo;
    int nRegistros;
    int nBlocos;
} RegistroCabecalho;

// constantes
const int TAM_BLOCO = 512;
const int TAM_REG = sizeof(Registro);
const int TAM_REG_CABECALHO = sizeof(RegistroCabecalho);

const int REG_BLOCO = TAM_BLOCO/TAM_REG;

const char PIPE = '|';

class Arquivo{
    private:
        const string nome;
        FILE * dados;
        RegistroCabecalho rcabecalho;
    public:
        Arquivo(string nomeArquivo);
        ~Arquivo();
        void insere(const Registro &);
        void lista();
        bool busca(const char *, Registro &);
};

Arquivo::Arquivo(string nomeArquivo): nome(nomeArquivo){
    this->dados = fopen(nomeArquivo.c_str(), "wb");
    this->rcabecalho.topo = -1;
    this->rcabecalho.nRegistros = 0;
    this->rcabecalho.nBlocos = 1;
    fwrite(&this->rcabecalho, TAM_REG_CABECALHO, 1, this->dados);

    // determinando bloco
    fseek(dados, TAM_BLOCO-1, SEEK_SET);
    fwrite(&PIPE, sizeof(PIPE), 1, dados);

    fclose(this->dados);

};

Arquivo::~Arquivo(){};

void Arquivo::insere(const Registro & reg){
    int rrn;
    char caracteresID[2];
    dados = fopen(nome.c_str(), "r+b");

    // verificar se há espaços vazios
    if(rcabecalho.topo == -1){ // se não há espaços vazios
        if(rcabecalho.nRegistros == REG_BLOCO){ //
            // criar novo bloco
            fseek(dados, TAM_BLOCO * (rcabecalho.nBlocos+1) - 1, SEEK_SET);
            fwrite(&PIPE, sizeof(PIPE), 1, dados);
            fseek(dados, -(TAM_BLOCO), SEEK_CUR);
            rcabecalho.nBlocos++;
        }else{
            if(rcabecalho.nBlocos == 1)
                fseek(dados, TAM_REG_CABECALHO + TAM_REG*rcabecalho.nRegistros, SEEK_SET);
            else{
                cout << "tadaima" << endl;
                fseek(dados, TAM_BLOCO*(rcabecalho.nRegistros/REG_BLOCO) + ((rcabecalho.nRegistros) % (REG_BLOCO))*TAM_REG, SEEK_SET);
            }
        }
            
        fwrite(&reg, TAM_REG, 1, dados);

    }else{ // se há
        if(rcabecalho.nBlocos == 1)
            fseek(dados, TAM_REG * rcabecalho.topo + TAM_REG_CABECALHO + sizeof('@'), SEEK_SET); // vá ao topo da lista
        else
            fseek(dados, TAM_BLOCO * (rcabecalho.topo/REG_BLOCO) + ((rcabecalho.topo) % (REG_BLOCO))  + sizeof('@'), SEEK_SET);

        fread(&rrn, sizeof(int), 1, dados); // guarde o próximo da lista (novo topo)
        rcabecalho.topo = rrn;
        fseek(dados, -(sizeof('@')+sizeof(int)), SEEK_CUR); // volte o ponteiro
        fwrite(&reg, TAM_REG, 1, dados); // escreva o registro
    }
    rcabecalho.nRegistros++;
    fclose(dados);
};

void Arquivo::lista(){
    Registro rAux;
    int i = 0;
    char ehArroba, ehPipe = 'a';

    dados = fopen(nome.c_str(), "rb");
    fseek(dados, TAM_REG_CABECALHO, SEEK_SET);
    // exibição sequencial dos dadoss
    while(1 == fread(&rAux, TAM_REG, 1, dados) && i < rcabecalho.nRegistros){ // se não chegou no fim do arquivo binário
        i++;
        // verificar se o registro lido está removido
        fseek(dados, -TAM_REG, SEEK_CUR);
        fread(&ehArroba, sizeof(char),1,dados);
        fseek(dados, TAM_REG-sizeof(char), SEEK_CUR);
        if(ehArroba != '@'){ // se nao achar '@', não está removido
            cout << endl;
            cout << rAux.cpf << endl;
            cout << rAux.nome << endl;
            cout << rAux.idade << endl;
            cout << endl;
        }else // se está removido
            cout << endl << "REMOVIDO" << endl << endl << endl << endl;
        

        if(i == REG_BLOCO) // se chegar no tamanho máximo de registros no bloco
            fseek(dados, TAM_BLOCO*(i/REG_BLOCO), SEEK_SET); // vá para o próximo
    }
    fclose(dados);
};
bool Arquivo::busca(const char * chave, Registro & R){
    //TODO: Tratar string com tamanho menor
    char * chaveAux = new char[12];
    int rrn = 0, posicao = -1;

    dados = fopen(nome.c_str(), "rb");
    fseek(dados, TAM_REG_CABECALHO, SEEK_SET);

    // busque sequencialmente no indice
    while(12 == fread(chaveAux, sizeof(char), 12, dados)){
        if(!strcmp(chaveAux, chave)) // se forem iguais (achou a chave)
            posicao = rrn; // guarde o RN da chave
        else
            rrn++;
        fseek(dados, TAM_REG - 12*sizeof(char), SEEK_CUR);
    }

    delete chaveAux;
    fclose(dados);

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