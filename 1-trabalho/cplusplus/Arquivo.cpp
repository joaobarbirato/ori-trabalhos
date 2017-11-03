
/*  
 *  TRABALHO 1: Implementação de arquivo sem ordenação
 *  DISCIPLINA: Organização e Recuperação da Informação
 *  DOCENTE: Ricardo Rodrigues Ciferri
 *  DISCENTES:  Gabrieli Santos                 RA: 726523
 *              João Gabriel Melo Barbirato     RA: 726546
 */
#ifndef ARQUIVO_HPP
#include <iostream>
#include <cstring>

using namespace std;

// Estrutura do registro
typedef struct{
    char cpf[12];
    char nome[51];
    int idade;
} Registro;

// Estrutura do registro de cabeçalho
typedef struct{
    int topo;
    int nRegistros;
    int nRetirados;
    int nBlocos;
} RegistroCabecalho;

// constantes
const int TAM_BLOCO = 512; // tamanho do bloco
const int TAM_REG = sizeof(Registro); // tamanho do registro
const int TAM_REG_CABECALHO = sizeof(RegistroCabecalho); // tamanho do registro de cabeçalho
const int REG_BLOCO = TAM_BLOCO/TAM_REG; /* quantos registros cabem num bloco (
                                         para o registro escolhido, serve tanto para o
                                         primeiro bloco (com registro de cabeçalho) quanto
                                         para os demais.
                                         */
const char PIPE = '|'; // constante delimitadora do bloco.

// Classe do arquivo de dados
class Arquivo{
    private:    // Componentes privados
        // atributos privados
        const string nome;      // nome do arquivo de dados
        FILE * dados;           // ponteiro para o arquivo de dados
        RegistroCabecalho rcabecalho; // cabeçalho do arquivo

        // métodos privados
        void atualizaRCabecalho(); // atualizar cabeçalho conforme mudado.

    public:     // Componentes públicos
        // métodos púlicos
        Arquivo(string nomeArquivo);    // construtor
        ~Arquivo();                     // destrutor
        void insere(const Registro &);  // inserção
        void lista();                   // listagem
        int busca(const char *, Registro &);   // busca
        void remove(const Registro &); // remoção
        void compacta(); // compactação
}; // fim Arquivo

/*  
 *  MÉTODO CONSTRUTOR
 *  OBJETIVO: Inicializar o arquivo de dados com o bloco inicial e o registro de cabeçalho.
 *  ARGUMENTOS:
 *      - nomeArquivo: nome do arquivo de dados
 */
Arquivo::Arquivo(string nomeArquivo): nome(nomeArquivo){
    this->dados = fopen(nomeArquivo.c_str(), "wb"); // determine do arquivo
    this->rcabecalho.topo = -1; // aponte o topo da lista invertida para nenhum registro
    this->rcabecalho.nRegistros = 0; // inicie o número de registros como 0
    this->rcabecalho.nBlocos = 1;   //  inicie o numero de blocos como 1
    this->rcabecalho.nRetirados = 0;
    fwrite(&this->rcabecalho, TAM_REG_CABECALHO, 1, this->dados); // escreva o registro de cabeçalho no arquivo de dados

    // determinando bloco
    fseek(dados, TAM_BLOCO-1, SEEK_SET); // delimite o bloco
    fwrite(&PIPE, sizeof(PIPE), 1, dados); // com um '|'

    fclose(this->dados); // feche o arquivo lógico

}; // fim construtor

/*  MÉTODO DESTRUTOR */
Arquivo::~Arquivo(){};

/*  
 *  MÉTODO insere
 *  OBJETIVO: Inserir registros no arquivo de dados
 *  ARGUMENTOS:
 *      - nomeArquivo: nome do arquivo de dados
 */
void Arquivo::insere(const Registro & reg){
    int rrn; // variável auxiliar para guardar o rrn quando o algoritmo seguir o fluxo da lista invertida

    dados = fopen(nome.c_str(), "r+b"); // abra o arquivo lógico

    // verifique se há espaços vazios no arquivo:
    if(rcabecalho.topo == -1){ // se não há espaços deletados:
        // posicione o ponteiro para o local adequado
        if(rcabecalho.nRegistros / REG_BLOCO == rcabecalho.nBlocos){ // se os blocos estão preenchidos
            // crie um novo bloco:
            fseek(dados, TAM_BLOCO * (rcabecalho.nBlocos+1) - 1, SEEK_SET); // delimite um novo bloco
            fwrite(&PIPE, sizeof(PIPE), 1, dados); // com o caractere PIPE
            fseek(dados, -(TAM_BLOCO), SEEK_CUR);  // volte o ponteiro para a posição onde será gravado o regitsro
            rcabecalho.nBlocos++; // aumente o número de blocos
        }else // se há espaços nos blocos:

            if(rcabecalho.nBlocos == 1) // se é o primeiro bloco (dentro há o registro de cabeçalho)
                // mova o ponteiro para depois do registro de cabeçalho
                fseek(dados, TAM_REG_CABECALHO + TAM_REG*rcabecalho.nRegistros, SEEK_SET);
            else // para demais blocos:
                 // mova o ponteiro para o espaço vazio mais próximo
                fseek(dados, TAM_BLOCO*(rcabecalho.nRegistros/REG_BLOCO) + ((rcabecalho.nRegistros) % (REG_BLOCO))*TAM_REG, SEEK_SET);
        
        // posicionado o ponteiro,
        fwrite(&reg, TAM_REG, 1, dados); // escreva no registro

    }else{ // se há espaços deletados:
        if(rcabecalho.nBlocos == 1) // se o arquivo tiver apenas o bloco inicial
            // vá ao topo da lista, pulando o registro de cabeçalho
            fseek(dados, TAM_REG * rcabecalho.topo + TAM_REG_CABECALHO + sizeof('@'), SEEK_SET);
        else // para demais blocos:
            // navegue pelos blocos até o topo da lista
            fseek(dados, TAM_BLOCO * (rcabecalho.topo/REG_BLOCO) + ((rcabecalho.topo) % (REG_BLOCO))  + sizeof('@'), SEEK_SET);

        // posicionado o ponteiro,
        fread(&rrn, sizeof(int), 1, dados); // guarde o próximo da lista (novo topo)
        rcabecalho.topo = rrn;
        fseek(dados, -(sizeof('@')+sizeof(int)), SEEK_CUR); // volte o ponteiro
        fwrite(&reg, TAM_REG, 1, dados); // escreva o registro
    }
    // inserido o registro
    rcabecalho.nRegistros++; // aumente o número de registros
    fclose(dados); // feche o arquivo lógico
    atualizaRCabecalho(); // e atualize o cabeçalho.
}; // fim insere

/*  
 *  MÉTODO lista
 *  OBJETIVO: Listar os registros no arquivo de dados, inclusive os removidos.
 */
void Arquivo::lista(){
    Registro rAux; // registro auxiliar para exibição dos dados do registro
    int contRRN = 0;  // auxiliar de contagem para controlar o RRN exibido
    char ehArroba;  // auxiliar para verificar se o registro está removido

    dados = fopen(nome.c_str(), "rb"); // abra o arquivo de dados
    fseek(dados, TAM_REG_CABECALHO, SEEK_SET); // posicione o ponteiro para após o registro de cabeçalho
    // exibição sequencial dos dados
    while(1 == fread(&rAux, TAM_REG, 1, dados) && contRRN < rcabecalho.nRegistros){ // se não chegou no fim do arquivo
        contRRN++;
        // verificar se o registro lido está removido
        fseek(dados, -TAM_REG, SEEK_CUR); //
        fread(&ehArroba, sizeof(char),1,dados);
        fseek(dados, TAM_REG-sizeof(char), SEEK_CUR);
        if(ehArroba != '@'){ // se nao achar '@', não está removido
            // exiba os dados do registro no terminal

// TODO: pensar numa maneira mais organizada de exibir os dados
            
            cout << endl;
            cout << rAux.cpf << endl;
            cout << rAux.nome << endl;
            cout << rAux.idade << endl;
            cout << endl;
        }else // se está removido, deixe isso explícito.
            cout << endl << "REMOVIDO" << endl << endl << endl << endl;

        if(contRRN == (rcabecalho.nBlocos-1)*REG_BLOCO) // se chegar no tamanho máximo de registros no bloco
            fseek(dados, TAM_BLOCO*(contRRN/REG_BLOCO), SEEK_SET); // vá para o próximo
    }
    fclose(dados);
}; // fim lista

//TODO: revisar busca
int Arquivo::busca(const char * chave, Registro & R){
    char * chaveAux = new char[12];
    int rrn = 0;
    int posicao = 0;
    int bloco_atual = 0;
    int nRegRemovidos = 0;

    dados = fopen(nome.c_str(), "rb");

    if(rcabecalho.nRegistros == 0){ //verificar se o arquivo é vazio
        delete chaveAux;
        fclose(dados);
        return -1;
    }else{ 
        while(bloco_atual != rcabecalho.nBlocos){
            if (bloco_atual == 0){
                fseek(dados, TAM_REG_CABECALHO, SEEK_SET);
                while(rcabecalho.nRegistros + nRegRemovidos != rrn && posicao/7 != 1){
                    fread(chaveAux, sizeof(char), 12, dados);
                    if(!strcmp(chaveAux, chave)){ //se forem iguais (achou chave)
                        fseek(dados, TAM_REG*posicao + TAM_REG_CABECALHO, SEEK_SET);
                        fread(&R, TAM_REG, 1, dados);
                        delete chaveAux;
                        fclose(dados);
                        return rrn;
                    }
                    else if(chaveAux[1] != '@'){ //registro nao foi removido
                        fseek(dados, -sizeof(chaveAux), SEEK_CUR);
                        rrn++;
                        posicao++;
                        fseek(dados, TAM_REG*posicao + TAM_REG_CABECALHO, SEEK_SET);
                    }
                    else if(chaveAux[1] == '@'){ //registro foi removido
                        fseek(dados, -sizeof(chaveAux), SEEK_CUR);
                        rrn++;
                        nRegRemovidos++;
                        posicao++;
                        fseek(dados, TAM_REG*posicao + TAM_REG_CABECALHO, SEEK_SET);
                    }
                }
                if(posicao/7 == 1){
                    bloco_atual++;
                }else if(posicao < 7){
                    cout << "registro nao encontrado!" << endl; 
                    delete chaveAux;
                    fclose(dados);
                    return -1;
                }
            }else{
                posicao = 0;
                fseek(dados, TAM_BLOCO*bloco_atual, SEEK_SET);
                while(rcabecalho.nRegistros + nRegRemovidos != rrn && posicao/7 != 1){
                    fread(chaveAux, sizeof(char), 12, dados);
                    if(!strcmp(chaveAux, chave)){ //se forem iguais (achou chave)
                        fseek(dados, TAM_REG*posicao + TAM_BLOCO*bloco_atual, SEEK_SET);
                        fread(&R, TAM_REG, 1, dados);
                        delete chaveAux;
                        fclose(dados);
                        //cout << "oi-4" << endl;
                        return rrn;
                    }else if(chaveAux[1] != '@'){ //registro nao foi removido
                        fseek(dados, -sizeof(chaveAux), SEEK_CUR);
                        rrn++;
                        posicao++;
                        fseek(dados, TAM_REG*posicao + TAM_BLOCO*bloco_atual, SEEK_SET);
                    }else if(chaveAux[1] == '@'){ //registro foi removido
                        fseek(dados, -sizeof(chaveAux), SEEK_CUR);
                        rrn++;
                        nRegRemovidos++;
                        posicao++;
                        fseek(dados, TAM_REG*posicao + TAM_BLOCO*bloco_atual, SEEK_SET);
                    }
                }
                if(posicao/7 == 1){
                    bloco_atual++;
                }else if(posicao < 7){
                    cout << "registro nao encontrado!" << endl;
                    delete chaveAux;
                    fclose(dados);
                    return -1;
                }
            }
        }
        delete chaveAux;
        fclose(dados);
        return -1;
    }
};

/*  
 *  MÉTODO remoção
 *  OBJETIVO: Remover registros no arquivo de dados
 *  ARGUMENTOS:
 *      - nomeArquivo: nome do arquivo de dados
 */
void Arquivo::remove(const Registro & reg){
    char arroba = '@';
    Registro rAux;
    int rrn;
    int posicao = 0;
    int bloco_atual;

    rrn = busca(reg.cpf, rAux);
    if(rrn == -1)
        return;

    dados = fopen(nome.c_str(), "r+b");
    if(rrn >= 0 && rrn < 7){ //se só existir um bloco
        fseek(dados, TAM_REG_CABECALHO + rrn*TAM_REG, SEEK_SET); //posiciona na posicao do resgistro a ser removido
        fwrite(&arroba, sizeof(arroba), 1, dados);
        fwrite(&rcabecalho.topo, sizeof(rcabecalho.topo), 1, dados);
        rcabecalho.topo = rrn;
    }
    else if(rrn >= 7){ //mais de um bloco
        
        for(int i=0; i<=rrn; i++){
            posicao++;
            if(posicao == 7){
                bloco_atual++;
                posicao = 0;
            }
        }
        fseek(dados, TAM_REG*posicao + TAM_BLOCO*bloco_atual, SEEK_SET);
        fwrite(&arroba, sizeof(arroba), 1, dados);
        fwrite(&rcabecalho.topo, sizeof(rcabecalho.topo), 1, dados);
        rcabecalho.topo = rrn;
    }
    // removendo o registro
    rcabecalho.nRegistros--; // diminua o número de registros
    rcabecalho.nRetirados++;
    fclose(dados); // feche o arquivo lógico
    atualizaRCabecalho(); // e atualize o cabeçalho.
}; //fim remoção

void Arquivo::compacta(){
    int topoAux; // elemento da lista
    int * vPilha;
    char * ehArroba;

    if(rcabecalho.topo == -1) // se o a lista está vazia
        return; // acabe o método
    else{ // se não
        vPilha = new int[rcabecalho.nRetirados];
        dados = fopen(nome.c_str(), "r+b");
        // transforme a pilha em um vetor
        topoAux = rcabecalho.topo;
        for(int i=0; i < rcabecalho.nRetirados; i++){
            if(topoAux/REG_BLOCO == 0) // se está no primeiro bloco
                fseek(dados, TAM_REG_CABECALHO + TAM_REG*topoAux, SEEK_SET);
            else // se está nos demais blocos
                fseek(dados, (topoAux/REG_BLOCO)*TAM_BLOCO + (topoAux % REG_BLOCO)*TAM_REG, SEEK_SET);
            fread(ehArroba, sizeof(char), 1, dados);

            if(*ehArroba != '@'){ // se removeu certo, nao vai chegar aqui. mas caso chegue:
                fclose(dados);
                return;
            }

            vPilha[i] = topoAux;
            fread(&topoAux, sizeof(int), 1, dados); // atualize o proximo topo
        }
    }
};

void Arquivo::atualizaRCabecalho(){
    dados = fopen(nome.c_str(), "r+b");
    fwrite(&rcabecalho, TAM_REG_CABECALHO, 1, dados);
    fclose(dados);
};

int main(){

    Arquivo arqDados("oimundo");
    Registro b;
    Registro a;
    Registro d;
    Registro c, aux;
    char p;
    sprintf(a.nome, "Gabrieli Santos");
    sprintf(a.cpf, "41458175839");
    a.idade = 20;

    sprintf(b.nome, "Gianna Barbirato");
    sprintf(b.cpf, "41193784468");
    b.idade = 55;

    sprintf(d.nome, "Micheli Santos");
    sprintf(d.cpf, "43810760870");
    d.idade = 17;

    for(int i=0; i < 7; i++){
        arqDados.insere(a);
    }
    arqDados.insere(b);
    arqDados.insere(b);
    arqDados.insere(d);
    arqDados.remove(a);
    arqDados.lista();

    /*if(arqDados.busca(d.cpf,c) != -1){
        cout << c.cpf << endl;
        cout << c.nome << endl;
        cout << c.idade << endl;
        cout << "RRN: " << arqDados.busca(d.cpf,c) << endl;
        cout << endl;
    }

    if(arqDados.busca(a.cpf,c) != -1){
        cout << c.cpf << endl;
        cout << c.nome << endl;
        cout << c.idade << endl;
        cout << "RRN: " << arqDados.busca(a.cpf,c) << endl;
        cout << endl;
    }
    
    if(arqDados.busca(b.cpf,c) != -1){
        cout << c.cpf << endl;
        cout << c.nome << endl;
        cout << c.idade << endl;
        cout << "RRN: " << arqDados.busca(b.cpf,c) << endl;
        cout << endl;
    }

    if(arqDados.busca("01010101010",c) != -1){
        cout << c.cpf << endl;
        cout << c.nome << endl;
        cout << c.idade << endl;
    }*/
   
    return 0;
}

#endif