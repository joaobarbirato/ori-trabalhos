#include <iostream>
#include "/Documents/ORI/git/ori-trabalhos/1-trabalho/cplusplus/src/Arquivo.hpp"

using namespace std;

int main(){
	int opcao; //variavel que guarda a opcao do menu
    int repete; //variavel que controla o do-while
    Arquivo arquivo("Arquivo de Dados");
    Registro registro;
    
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
	
	return EXIT_SUCCESS;
}