#ifndef INDICE_HPP
#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "src/Arquivo.hpp"

using namespace std;

#define MAX_INDICE_AUXILIAR 2

struct RegistroIndice{
	int chave, byteoffset;

	bool operator<(const RegistroIndice& r) const{
		return chave < r.chave;
	}
};


class Indice{
    private:    // Componentes privados
        // atributos privados
        string arquivo_principal = "'Arquivo de Dados'";      // nome do arquivo de dados
        vector<RegistroIndice> indicePrincipal, indiceAuxiliar;

        // métodos privados
		int compare(RegistroIndice a, RegistroIndice b);
		void printVector(vector<RegistroIndice> v);
		RegistroIndice *buscaSequencial(vector<RegistroIndice> v, int chave);
		void buscarNoIndice(int chave);

    public:     // Componentes públicos
        // métodos púlicos
		void insereNoIndice(int chave, int byteoffset);

}; // fim Arquivo


int compare(RegistroIndice a, RegistroIndice b){
	return (a.chave < b.chave);
}

void printVector(vector<RegistroIndice> v){
		for (int i=0; i<v.size(); i++){
		printf ("C: %d e B: %d\n", v[i].chave, v[i].byteoffset);
	}
}

RegistroIndice *buscaSequencial(vector<RegistroIndice> v, int chave){
	int i;

	for (i=0; i<v.size(); i++){
		if (v[i].chave == chave){
			return (RegistroIndice*) &v[i];
		}
	}
	return NULL;
}

void buscarNoIndice(int chave){

	 // vector<RegistroIndice>::iterator it = 
	RegistroIndice *reg = buscaSequencial(indicePrincipal, chave);

	if (reg == NULL){
		//Nao achou
		printf ("Registro nao encontrado!\n");
	} else {
		printf ("Encontrei\n");

		FILE *fp = fopen(arquivo_principal.c_str(), "r+");

		fseek(fp, reg->byteoffset, SEEK_SET);
		
		Registro R;
		fread(&R, TAM_REG, 1, fp); //passa por referencia o registro encontrado

		cout << "Registro encontrado!" << endl;
		cout << "Nome: "<< R.nome << endl;
		cout << "CPF: " << R.cpf << endl;
		cout << "Idade: "<< R.idade << endl << endl;
	}

	// if (it != indicePrincipal.end()){
	// 	//Achei	// 	FILE *fp = fopen(arquivo_indice.c_str(), "r+");

	// 	fseek(fp, (*it).byteoffset, SEEK_SET);
	// 	Registro R;
	// 	fread(&R, TAM_REG, 1, fp); //passa por referencia o registro encontrado


	// } else {
	// 	cout << "Registro nao encontrado!" << endl;
	// }
}

void insereNoIndice(int chave, int byteoffset){



	RegistroIndice novaInstancia;
	novaInstancia.chave = chave;
	novaInstancia.byteoffset = byteoffset;
	
	indiceAuxiliar.push_back(novaInstancia);

	//Passa tudo para o indice principal e ordena
	if (indiceAuxiliar.size() > MAX_INDICE_AUXILIAR){
		indicePrincipal.insert(indicePrincipal.end(), indiceAuxiliar.begin(), indiceAuxiliar.end());
		indiceAuxiliar.clear();
		sort(indicePrincipal.begin(), indicePrincipal.end());

		printf ("Colocou no indice principal\n");
		printVector(indicePrincipal);
	}
}

#endif