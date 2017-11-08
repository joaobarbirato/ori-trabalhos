
/*  
 *  TRABALHO 1: Implementação de arquivo sem ordenação
 *  DISCIPLINA: Organização e Recuperação da Informação
 *  DOCENTE: Ricardo Rodrigues Ciferri
 *  DISCENTES:  Gabrieli Santos                 RA: REG_BLOCO26523
 *              João Gabriel Melo Barbirato     RA: REG_BLOCO26546
 */
#ifndef ARQUIVO_HPP
#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>

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
    int nRemovidos;
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
        void remove_rrn(const int &, Registro &);

    public:     // Componentes públicos
        // métodos púlicos
        Arquivo(string nomeArquivo);    // construtor
        ~Arquivo();                     // destrutor
        void insere(const Registro &);  // inserção
        void lista();                   // listagem
        int busca(const char *, Registro &);   // busca
        void remove(const char * chave, Registro &); // remoção
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
    this->rcabecalho.nRemovidos = 0;
    fwrite(&this->rcabecalho, TAM_REG_CABECALHO, 1, this->dados); // escreva o registro de cabeçalho no arquivo de dados

    // determinando bloco
    fseek(dados, TAM_BLOCO-1, SEEK_SET); // delimite o bloco
    fwrite(&PIPE, sizeof(PIPE), 1, dados); // com um '|'

    fclose(this->dados); // feche o arquivo lógico

}; // fim CONSTRUTOR

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
        if(rcabecalho.topo / REG_BLOCO == 0) // se o arquivo tiver apenas o bloco inicial
            // vá ao topo da lista, pulando o registro de cabeçalho
            fseek(dados, TAM_REG * rcabecalho.topo + TAM_REG_CABECALHO + sizeof('@'), SEEK_SET);
        else{ // para demais blocos:
            // navegue pelos blocos até o topo da lista
            fseek(dados, TAM_BLOCO * (rcabecalho.topo/REG_BLOCO) + ((rcabecalho.topo) % (REG_BLOCO))*TAM_REG  + sizeof('@'), SEEK_SET);
            cout << "TADAIMA!" << endl;
        }
        // posicionado o ponteiro,
        fread(&rrn, sizeof(int), 1, dados); // guarde o próximo da lista (novo topo)
        rcabecalho.topo = rrn;
        fseek(dados, -(sizeof('@')+sizeof(int)), SEEK_CUR); // volte o ponteiro
        fwrite(&reg, TAM_REG, 1, dados); // escreva o registro
        rcabecalho.nRemovidos--;
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
    while(1 == fread(&rAux, TAM_REG, 1, dados) && contRRN < rcabecalho.nRegistros + rcabecalho.nRemovidos){ // se não chegou no fim do arquivo
        contRRN++;
        // verificar se o registro lido está removido
        fseek(dados, -TAM_REG, SEEK_CUR); //
        fread(&ehArroba, sizeof(char),1,dados);
        fseek(dados, TAM_REG-sizeof(char), SEEK_CUR);
        if(ehArroba != '@'){ // se nao achar '@', não está removido
            // exiba os dados do registro no terminal

            // TODO: pensar numa maneira mais organizada de exibir os dados
            
            cout << endl;
            cout << rAux.nome << endl;
            cout << rAux.cpf << endl;
            cout << rAux.idade << endl;
            cout << endl;
        }else // se está removido, deixe isso explícito.
            cout << endl << "REMOVIDO" << endl << endl << endl << endl;

        if(contRRN == (rcabecalho.nBlocos-1)*REG_BLOCO) // se chegar no tamanho máximo de registros no bloco
            fseek(dados, TAM_BLOCO*(contRRN/REG_BLOCO), SEEK_SET); // vá para o próximo
        //cout << contRRN << endl;
    }
    fclose(dados);
}; // fim lista

/*  
 *  MÉTODO busca
 *  OBJETIVO: Buscar registros no arquivo de dados a partir de uma chave
 *  ARGUMENTOS:
 *      - chave
 *      - nomeArquivo: nome do arquivo de dados
 */
int Arquivo::busca(const char * chave, Registro & R){
    char * chaveAux = new char[12]; //ponteiro que aponta para um vetor de char que guardará os 12 primeiros bytes de um registro
    int rrn = 0;
    int posicao = 0;        //variavel que percorre o bloco (seu valor é zero no início de cada bloco)
    int bloco_atual = 0;

    dados = fopen(nome.c_str(), "rb");

    if(rcabecalho.nRegistros == 0){ //verificar se o arquivo é vazio
        delete chaveAux;
        fclose(dados);
        return -1; //o registro nao existe no arquivo, portanto nao foi encontrado
    }else{ //se o arquivo nao estiver vazio
        while(bloco_atual != rcabecalho.nBlocos){ //cada repetição percorre um bloco diferente do arquivo
            if (bloco_atual == 0){ //controle de qual bloco esta sendo percorrido
                fseek(dados, TAM_REG_CABECALHO, SEEK_SET); //posiciona a posicao corrente no primeiro registro
                while(rcabecalho.nRegistros + rcabecalho.nRemovidos != rrn && posicao/REG_BLOCO != 1){ //estrutura usada para fazer a busca no primeiro bloco, que contém o registro de cabeçalho
                    fread(chaveAux, sizeof(char), 12, dados); //guarda o cpf de cada registro na variavel chaveAux
                    if(!strcmp(chaveAux, chave)){ //se forem iguais (achou chave)
                        fseek(dados, TAM_REG*posicao + TAM_REG_CABECALHO, SEEK_SET); //posicao corrente vai para o inicio do registro encontrado
                        fread(&R, TAM_REG, 1, dados); //passa por referencia o registro encontrado
                       // cout << "Registro encontrado!" << endl;
                       // cout << "Nome: "<< R.nome << endl;
                       // cout << "CPF: " << R.cpf << endl;
                       // cout << "Idade: "<< R.idade << endl << endl;
                        delete chaveAux;
                        fclose(dados);
                        return rrn;
                    }
                    else{ //se nao forem iguais (não achou a chave)
                        fseek(dados, -sizeof(chaveAux), SEEK_CUR); //volta para o inicio do registro
                        rrn++;
                        posicao++;
                        fseek(dados, TAM_REG*posicao + TAM_REG_CABECALHO, SEEK_SET); //posição corrente vai para o proximo registro
                    }
                }
                if(posicao/REG_BLOCO == 1){ //se o primeiro bloco ja tiver sido percorrido e chave ainda nao foi encontrada
                    bloco_atual++; //proximo bloco
                }else if(posicao < REG_BLOCO){ //se o primeiro bloco nao possui mais registros e a chave ainda nao foi encontrada
                    cout << "Registro nao encontrado!" << endl; 
                    delete chaveAux;
                    fclose(dados);
                    return -1; //o registro não existe no arquivo, portanto não foi encontrado
                }
            }
            else{ //se nao está no primeiro bloco
                posicao = 0; //variavel que percorre o bloco é zerada, indicando inicio do novo bloco
                fseek(dados, TAM_BLOCO*bloco_atual, SEEK_SET); //posicao corrente vai para o inicio do bloco
                while(rcabecalho.nRegistros + rcabecalho.nRemovidos != rrn && posicao/REG_BLOCO != 1){ //estrutura usada para fazer a busca nos demais blocos, que não contém o registro de cabeçalho
                    fread(chaveAux, sizeof(char), 12, dados); //guarda o cpf de cada registro na variavel chaveAux
                    if(!strcmp(chaveAux, chave)){ //se forem iguais (achou chave)
                        fseek(dados, TAM_REG*posicao + TAM_BLOCO*bloco_atual, SEEK_SET); //posicao corrente vai para o inicio do registro encontrado
                        fread(&R, TAM_REG, 1, dados); //passa por referencia o registro encontrado
                        /*
                        cout << "Registro encontrado!" << endl;
                        cout << "Nome: "<< R.nome << endl;
                        cout << "CPF: " << R.cpf << endl;
                        cout << "Idade: "<< R.idade << endl << endl;
                        */
                        delete chaveAux;
                        fclose(dados);
                        return rrn;
                    }
                    else{ //se nao forem iguais (não achou a chave)
                        fseek(dados, -sizeof(chaveAux), SEEK_CUR); //volta para o inicio do registro
                        rrn++;
                        posicao++;
                        fseek(dados, TAM_REG*posicao + TAM_BLOCO*bloco_atual, SEEK_SET); //posição corrente vai para o proximo registro
                    }
                }
                if(posicao/REG_BLOCO == 1){
                    bloco_atual++;
                }else if(posicao < REG_BLOCO){
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
void Arquivo::remove(const char * chave, Registro & reg){
    char arroba = '@';
    Registro rAux;
    int rrn;

    rrn = busca(chave, rAux);
    if(rrn == -1)
        return;

    dados = fopen(nome.c_str(), "r+b");
    if(rrn >= 0 && rrn < REG_BLOCO) //se só existir um bloco
        fseek(dados, TAM_REG_CABECALHO + rrn*TAM_REG, SEEK_SET); //posiciona na posicao do resgistro a ser removido

    else if(rrn >= REG_BLOCO) //se o registro estiver a partir do segundo bloco
        fseek(dados, (TAM_BLOCO)*(rrn/REG_BLOCO) + (TAM_REG)*(rrn % REG_BLOCO), SEEK_SET); //posiciona no inicio de cada bloco
    
    fwrite(&arroba, sizeof(arroba), 1, dados); //escreve o caractere '@' no primeiro byte do registro
    fwrite(&rcabecalho.topo, sizeof(rcabecalho.topo), 1, dados); //escreve o topo da lista invertida nos quatro bytes seguintes criando uma lista invertida encadeada
    rcabecalho.topo = rrn; //atualiza o topo
    // removendo o registro
    rcabecalho.nRegistros--; // diminua o número de registros
    rcabecalho.nRemovidos++;
    fclose(dados); // feche o arquivo lógico
    atualizaRCabecalho(); // e atualize o cabeçalho.
}; //fim remoção

void Arquivo::remove_rrn(const int & rrn, Registro & reg){
    char arroba = '@';
    Registro rAux;

    if(rrn == -1)
        return;

    dados = fopen(nome.c_str(), "r+b");
    if(rrn >= 0 && rrn < REG_BLOCO) //se só existir um bloco
        fseek(dados, TAM_REG_CABECALHO + rrn*TAM_REG, SEEK_SET); //posiciona na posicao do resgistro a ser removido

    else if(rrn >= REG_BLOCO) //se o registro estiver a partir do segundo bloco
        fseek(dados, (TAM_BLOCO)*(rrn/REG_BLOCO) + (TAM_REG)*(rrn % REG_BLOCO), SEEK_SET); //posiciona no inicio de cada bloco
    
    fwrite(&arroba, sizeof(arroba), 1, dados); //escreve o caractere '@' no primeiro byte do registro
    fwrite(&rcabecalho.topo, sizeof(rcabecalho.topo), 1, dados); //escreve o topo da lista invertida nos quatro bytes seguintes criando uma lista invertida encadeada
    rcabecalho.topo = rrn; //atualiza o topo
    // removendo o registro
    rcabecalho.nRegistros--; // diminua o número de registros
    rcabecalho.nRemovidos++;
    fclose(dados); // feche o arquivo lógico
    atualizaRCabecalho(); // e atualize o cabeçalho.
}; //fim remoção_rrn

void Arquivo::compacta(){
    Registro rAux;          // aux pra remoção
    RegistroCabecalho rcAux;
    //RegistroCabecalho rcabAux;
    FILE * dadosTemp;
    Registro * bufferRegistros;
    RegistroCabecalho bufferCabecalho;
    int proxTopo;
    int novoNBlocos, novoNRegistros, novoNRemovidos;
    char ehArroba;
    char arroba = '@';
    int i = 0, j=0, rrnUltimo;
    bool * estaCompactado;      // vetor para verificar se o arquivo já está compactado
    bool estaCompactadoAnd;     // and de todos os estacompactado
    std::vector<int> ultimosRegistros, rrnRemovidos;
    if(rcabecalho.topo == -1) // se a lista está vazia
        return ; // acabe o método

    estaCompactado = new bool[rcabecalho.nRemovidos];

    dados = fopen(nome.c_str(), "r+b");

    // verifique se já está compactado
    rrnUltimo = rcabecalho.nRegistros + rcabecalho.nRemovidos - 1;
    while(i < rcabecalho.nRemovidos){
        if(rrnUltimo/REG_BLOCO == 0) // se está no primeiro bloco
            fseek(dados, TAM_REG_CABECALHO + TAM_REG*rrnUltimo, SEEK_SET);
        else // se está nos demais blocos
            fseek(dados, (rrnUltimo/REG_BLOCO)*TAM_BLOCO + (rrnUltimo % REG_BLOCO)*TAM_REG, SEEK_SET);
        fread(&ehArroba, sizeof(char), 1, dados);
        if(ehArroba != '@'){
            i++;
            ultimosRegistros.push_back(rrnUltimo);
            estaCompactado[j] = false;
        }else{
            rrnRemovidos.push_back(rrnUltimo);
            estaCompactado[j] = true;
        }
        j++;
        rrnUltimo--;
    }
    rrnRemovidos.insert(rrnRemovidos.begin(), -1);
    estaCompactadoAnd = estaCompactado[0];
    for(int i=1; i < rcabecalho.nRemovidos; i++)
        estaCompactadoAnd = estaCompactadoAnd && estaCompactado[i];

    if(!estaCompactadoAnd){ // não está compactado
        for(int i=0, j=0; i< ultimosRegistros.size(); i++){
            if(ultimosRegistros[j] > rrnRemovidos.back()){
                // extraia um dos últimos registros
                if(ultimosRegistros[j]/REG_BLOCO == 0) // se está no primeiro bloco
                    fseek(dados, TAM_REG_CABECALHO + TAM_REG*ultimosRegistros[j], SEEK_SET);
                else // se está nos demais blocos
                    fseek(dados, (ultimosRegistros[j]/REG_BLOCO)*TAM_BLOCO + (ultimosRegistros[j] % REG_BLOCO)*TAM_REG, SEEK_SET);
                fread(&rAux, TAM_REG, 1, dados);
        
                // remova logicamente esse ultimo registro
                fseek(dados, -TAM_REG, SEEK_CUR);
                fwrite(&arroba, sizeof(char), 1, dados);
                fwrite(&rrnRemovidos[i], sizeof(int), 1, dados);
    
                // insira no registro
                if(rrnRemovidos.back() / REG_BLOCO == 0) // se o arquivo tiver apenas o bloco inicial
                    // vá ao topo da lista, pulando o registro de cabeçalho
                    fseek(dados, TAM_REG * rrnRemovidos.back() + TAM_REG_CABECALHO + sizeof('@'), SEEK_SET);
                else // para demais blocos:
                    // navegue pelos blocos até o topo da lista
                    fseek(dados, TAM_BLOCO * (rrnRemovidos.back()/REG_BLOCO) + (rrnRemovidos.back() % REG_BLOCO)*TAM_REG + sizeof('@'), SEEK_SET);
                // posicionado o ponteiro,
                fread(&rrnUltimo, sizeof(int), 1, dados); // guarde o próximo da lista (novo topo)
                //cout << rrnUltimo << endl << endl;
                rcabecalho.topo = rrnUltimo;
                fseek(dados, -(sizeof('@')+sizeof(int)), SEEK_CUR); // volte o ponteiro
                fwrite(&rAux, TAM_REG, 1, dados); // escreva o registro
                j++;
            }
            rrnRemovidos.pop_back();
        }
        cout << rcabecalho.topo << endl;
        rcabecalho.topo = rrnRemovidos[i];
    }
    delete [] estaCompactado;
    // se é necessário deletar blocos:
    if(rcabecalho.nRemovidos >= (rcabecalho.nRemovidos + rcabecalho.nRegistros)%REG_BLOCO){
        novoNBlocos = (rcabecalho.nBlocos-(rcabecalho.nRegistros + rcabecalho.nRemovidos)/REG_BLOCO);
        novoNRegistros = rcabecalho.nRegistros;
        novoNRemovidos = rcabecalho.nRemovidos - (rcabecalho.nRemovidos + rcabecalho.nRegistros)%REG_BLOCO;

        bufferRegistros = new Registro[REG_BLOCO];

        bufferCabecalho.topo = rcabecalho.topo;
        bufferCabecalho.nBlocos = novoNBlocos;
        bufferCabecalho.nRemovidos = novoNRemovidos;
        bufferCabecalho.nRegistros = novoNRegistros;
        // guarde tudo num arquivo temporário        
        dadosTemp = fopen((nome + "-aux").c_str(), "w+b");
        for(int i=0; i < novoNBlocos; i++){
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TEM ALGUM ERRO NESSES FSEEK FWRITE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            if(i == 0){ // se está copiando do primeiro bloco
                fseek(dadosTemp, 0, SEEK_SET);
                fwrite(&bufferCabecalho, TAM_REG_CABECALHO, 1, dadosTemp);
                fseek(dados, TAM_REG_CABECALHO, SEEK_SET);

            }else{ // se está copiando em demais blocos
                fseek(dadosTemp, i*TAM_BLOCO, SEEK_SET);
                fseek(dados, i*TAM_BLOCO, SEEK_SET);
            }
            fread(&rAux, TAM_REG, 1, dados);
            cout << rAux.cpf << endl;
            cout << rAux.nome << endl;
            cout << rAux.idade << endl;
            fwrite(bufferRegistros, TAM_REG, REG_BLOCO, dadosTemp);
            fseek(dadosTemp, TAM_REG*(1-REG_BLOCO) - 1, SEEK_CUR);
            fwrite(&PIPE, sizeof(char), 1, dadosTemp);
        }
        fclose(dadosTemp);
        std::rename((nome+"-aux").c_str(), nome.c_str()); // remova o arquivo auxiliar
            

        //fwrite(bufferBloco, TAM_BLOCO*(rcabecalho.nBlocos-(rcabecalho.nRegistros + rcabecalho.nRemovidos)/REG_BLOCO), 1, dados);
    }
    fclose(dados);
};

void Arquivo::atualizaRCabecalho(){
    dados = fopen(nome.c_str(), "r+b");
    fwrite(&rcabecalho, TAM_REG_CABECALHO, 1, dados);
    fclose(dados);
};

int main(){
    int opcao; //variavel que guarda a opcao do menu
    int repete; //variavel que controla o do-while
    Arquivo arquivo("Arquivo de Dados");
    Registro registro;
    
    Registro b;
    Registro a;
    Registro d;
    Registro c, aux;
    
    sprintf(a.nome, "Gabrieli Santos");
    sprintf(a.cpf, "41458175839");
    a.idade = 20;

    sprintf(b.nome, "Gianna Barbirato");
    sprintf(b.cpf, "01010101010");
    b.idade = 55;

    sprintf(d.nome, "Micheli Santos");
    sprintf(d.cpf, "02020202020");
    d.idade = 17;
    arquivo.insere(a);
    arquivo.insere(a);
    arquivo.insere(a);
    arquivo.insere(a);
    arquivo.insere(a);
    arquivo.insere(a);
    arquivo.insere(a);
    arquivo.insere(b);
    arquivo.insere(d);
    do{
        cout << "***MENU DE OPCOES***" << endl << endl;
        cout << "1. Inserir registro" << endl;
        cout << "2. Buscar registro" << endl;
        cout << "3. Remover registro" << endl;
        cout << "4. Listar registros" << endl;
        cout << "5. Compactar arquivo" << endl << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cout << endl;

        switch (opcao){
            case 1:
                cout << "Nome: ";
                cin.ignore(51, '\n');
                cin.get(registro.nome, 51);
                cout << endl;
                cout << "CPF: ";
                cin >> registro.cpf;
                cout << endl;
                cout << "Idade: ";
                cin >> registro.idade;
                cout << endl;
                arquivo.insere(registro);
                cout << "O registro foi inserido no Arquivo de Dados!" << endl << endl;
                break;
            case 2:
                cout << "Informe o CPF: ";
                cin >> registro.cpf;
                cout << endl;
                arquivo.busca(registro.cpf, registro);
                break;
            case 3:
                cout << "Informe o CPF: ";
                cin >> registro.cpf;
                cout << endl;
                arquivo.remove(registro.cpf, registro);
                cout << "O registro foi removido do Arquivo de Dados" << endl;
                break;
            case 4:
                arquivo.lista();
                break;
            case 5:
                arquivo.compacta();
                cout << "O arquivo foi compactado!" << endl;
                break;
            default:
                cout << "Opcao invalida !!" << endl;
                cin.ignore(256, '\n');
        }

        cout << "Voltar ao MENU DE OPCOES? [1 para SIM/2 para NAO]: ";
        cin >> repete;
        cout << endl;
    } while (repete == 1);
   
    return 0;
}

#endif