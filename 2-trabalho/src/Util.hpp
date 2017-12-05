#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>      // std::ifstream

#include "../src/Arquivo.hpp"
#include "../src/Indice.hpp"

using namespace std;

class Util{
    
    public:
    void paraCSV(Indice *indice);
    int deCSV(Indice *indice, Arquivo *arquivo);

};

/*
Funcao usada para transformar os dados existentes em uma tabela csv
*/
void Util::paraCSV(Indice *indice){
    FILE *dataFile = fopen("dados", "r");
    FILE *csvFile = fopen("dados.csv", "w");

    
    indice->unificarIndices();

    vector<RegistroIndice> v = indice->indicePrincipal;

    Registro R;
    RegistroIndice regIdx;

    fprintf(csvFile, "%s,%s,%s", "CPF", "Nome", "Idade");
    for (int i=0; i<v.size(); i++){
        regIdx = v[i];

        fseek(dataFile, regIdx.byteoffset, SEEK_SET);
        fread(&R, TAM_REG, 1, dataFile);

        fprintf(csvFile, "\n%s,%s,%d", R.cpf, R.nome, R.idade);
    }

    fclose(csvFile);
    fclose(dataFile);
}

/*
Funcao que le uma tabela CSV, e adiciona ao arquivo
*/
int Util::deCSV(Indice *indice, Arquivo *arquivo){
    ifstream csvFile ("dados.csv", ifstream::in);
    if (!csvFile) return -1;

    Registro R;
    int instancias=0;

    string str;
    char *pch;
    
    getline(csvFile, str);
    while (getline(csvFile, str)){
        pch = strtok ((char*)str.c_str(),",");
        strcpy(R.cpf, pch);
        pch = strtok (NULL, ",");
        strcpy(R.nome, pch);
        pch = strtok (NULL, ",");
        R.idade = atoi(pch);
        
        instancias++;

        int byteoffset = arquivo->insere(R);
        indice->insereNoIndice(atoi(R.cpf), byteoffset);
    }

    csvFile.close();
    return instancias;
}

#endif