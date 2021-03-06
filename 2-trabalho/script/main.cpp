#include <iostream>
#include <stdlib.h>
#include "../src/Arquivo.hpp"
#include "../src/Indice.hpp"
#include "../src/Util.hpp"

using namespace std;

int main(){
	int opcao; //variavel que guarda a opcao do menu
    int repete; //variavel que controla o do-while
    Arquivo arquivo("dados");
    Indice indice;
    Util util;
    int byteoffset;

    Registro registro;
    Registro *R;
    
    int instancias = util.deCSV(&indice, &arquivo);
    if (instancias > 0) cout << "Arquivo CSV carregou " << instancias << " instancias\n" << endl;

    do{
        cout << "***MENU DE OPCOES***" << endl << endl;
        cout << "1. Inserir registro" << endl;
        cout << "2. Buscar registro" << endl;
        cout << "3. Remover registro" << endl;
        cout << "4. Listar registros" << endl;
        cout << "5. Compactar arquivo" << endl;
        cout << "6. Exportar CSV" << endl << endl;
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
                byteoffset = arquivo.insere(registro);
                indice.insereNoIndice(atoi(registro.cpf), byteoffset);

                cout << "O registro foi inserido no Arquivo de Dados!" << endl << endl;
                break;
            case 2:
                cout << "Informe o CPF: ";
                cin >> registro.cpf;
                cout << endl;
                // arquivo.busca(registro.cpf, registro);
                R = indice.buscarNoIndice(atoi(registro.cpf));

                if (R==NULL){
                    printf ("Registro nao encontrado!\n");
                } else {
                    cout << "Registro encontrado!" << endl;
                    cout << "Nome: "<< R->nome << endl;
                    cout << "CPF: " << R->cpf << endl;
                    cout << "Idade: "<< R->idade << endl << endl;
                }

                break;
            case 3:
                cout << "Informe o CPF: ";
                cin >> registro.cpf;
                cout << endl;
                arquivo.remove(registro.cpf, registro);
                indice.remover(atoi(registro.cpf));
                cout << "O registro foi removido do Arquivo de Dados" << endl;
                break;
            case 4:
                arquivo.lista();
                break;
            case 5:
                arquivo.compacta();
                cout << "O arquivo foi compactado!" << endl;
                break;
            case 6:
                util.paraCSV(&indice);
                cout << "O arquivo foi exportado!" << endl;
                break;
            default:
                cout << "Opcao invalida !!" << endl;
                cin.ignore(256, '\n');
        }

        cout << "Voltar ao MENU DE OPCOES? [1 para SIM/2 para NAO]: ";
        cin >> repete;
        cout << endl;
    } while (repete == 1);
	
	return EXIT_SUCCESS;
}