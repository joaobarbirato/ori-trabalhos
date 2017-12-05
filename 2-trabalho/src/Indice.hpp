#ifndef INDICE_HPP
#define INDICE_HPP
#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "Arquivo.hpp"

using namespace std;

#define MAX_INDICE_AUXILIAR 5

struct RegistroIndice{
	int chave, byteoffset;

	bool operator<(const RegistroIndice& r) const{
		return chave < r.chave;
	}
};

class Indice{
    private: // Componentes privados
        // atributos privados
		string arquivo_principal = "dados";      // nome do arquivo de dados

        // métodos privados
		int buscaSequencial(vector<RegistroIndice> v, int chave);
		int buscaBinaria(vector<RegistroIndice> v, int chave);
		int bin(vector<RegistroIndice> v, int chave, int lower, int upper);

    public: // Componentes públicos


        // métodos púlicos
		Registro *buscarNoIndice(int chave);
		void insereNoIndice(int chave, int byteoffset);
		void remover(int chave);
		void unificarIndices();
		vector<RegistroIndice> indicePrincipal, indiceAuxiliar;
		void printVector(vector<RegistroIndice> v);

}; // fim Arquivo

void Indice::printVector(vector<RegistroIndice> v){
		for (int i=0; i<v.size(); i++){
		printf ("C: %d e B: %d\n", v[i].chave, v[i].byteoffset);
	}
}

//Realiza busca sequencial em um vetor
int Indice::buscaSequencial(vector<RegistroIndice> v, int chave){

	for (int i=0; i<v.size(); i++){
		if (v[i].chave == chave){
			return i;
		}
	}
	return -1;
}

//Realiza a busca binaria em um vetor
int Indice::bin(vector<RegistroIndice> v, int chave, int lower, int upper){

	if (lower > upper) return -1;

	int mid = (lower + upper)/2;
	if (v[mid].chave == chave){
		return mid;
	} else if (v[mid].chave > chave){
		return bin (v, chave, lower, mid-1);
	} else {
		return bin (v, chave, mid+1, upper);
	}
}

int Indice::buscaBinaria(vector<RegistroIndice> v, int chave){
	return bin(v, chave, 0, v.size()-1);
}

/*
Funcao que faz a busca de uma chave no indice
Primeiro tenta encontrar no IndicePrincipal (Geralmente grande e sempre ordenado) usando busca binaria
Se nao encontrar, faz uma busca sequencial no pequeno indice auxiliar
*/
Registro *Indice::buscarNoIndice(int chave){

	 // vector<RegistroIndice>::iterator it = 
	int idx = buscaBinaria(indicePrincipal, chave);
	RegistroIndice reg;
	if (idx < 0){

		idx = buscaSequencial(indiceAuxiliar, chave);
		if (idx < 0){
			//Nao achou
			return NULL;
		} else {
			reg = indiceAuxiliar[idx];
		}
	} else {
		reg = indicePrincipal[idx];
	}
	FILE *fp = fopen(arquivo_principal.c_str(), "r+");
	fseek(fp, reg.byteoffset, SEEK_SET);
	
	Registro *R = (Registro*) malloc (sizeof(Registro));
	fread(R, TAM_REG, 1, fp); //passa por referencia o registro encontrado

	return R;
}

/*Insere no indice
Inicialmente, insere no indice auxiliar, que nao precisa de ordenacao
Assim que o indice auxiliar estiver grande o suficiente, ele sera unificado
ao indice principal, e todos os valores serao ordenados
*/
void Indice::insereNoIndice(int chave, int byteoffset){

	if (buscarNoIndice(chave) != NULL){
		cout << "CPF ja cadastrado!\n" << endl;
		return;
	}

	RegistroIndice novaInstancia;
	novaInstancia.chave = chave;
	novaInstancia.byteoffset = byteoffset;

	
	indiceAuxiliar.push_back(novaInstancia);

	//Passa tudo para o indice principal e ordena
	if (indiceAuxiliar.size() > MAX_INDICE_AUXILIAR){
		unificarIndices();
	}
}

/*
Unifica o indice principal com o auxiliar, e ordena tudo
*/
void Indice::unificarIndices(){

	cout << "O indice auxiliar sera unificado ao indice principal" << endl;

	indicePrincipal.insert(indicePrincipal.end(), indiceAuxiliar.begin(), indiceAuxiliar.end());
	indiceAuxiliar.clear();
	sort(indicePrincipal.begin(), indicePrincipal.end());
}

/*
Remove uma chave do indice
*/
void Indice::remover(int chave){
	int idx = buscaBinaria(indicePrincipal, chave);
	if (idx >= 0){
		indicePrincipal.erase(indicePrincipal.begin() + idx);
	} else {
		idx = buscaSequencial(indiceAuxiliar, chave);
		if (idx >= 0){
			indiceAuxiliar.erase(indiceAuxiliar.begin() + idx);
		}
	}
}

#endif