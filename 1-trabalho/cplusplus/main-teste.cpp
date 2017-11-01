#include <iostream>
#include "Arquivo.hpp"

using namespace std;

int main(){

	/*
	Bloco b;
	Registro r, a;
	sprintf(r.cpf, "43710760870");
	sprintf(r.nome, "Gabriela Purini Chermonte");
	sprintf(r.outro, "e eu vou saber");
	r.idade = 20;

	FILE * teste = fopen("oimundo","rb");
	fread(&b, TAM_BLOCO, 1, teste);
	arqDados.insere(r);
	fseek(teste, TAM_BLOCO_CABECALHO, SEEK_SET);
	fread(&a, TAM_BLOCO, 1, teste);

	cout << a.cpf << endl;
	cout << a.nome << endl;
	cout << a.outro << endl;
	cout << a.idade << endl;

*/

	

	Arquivo arqDados("oimundo");
	Registro b;
	Registro a;
	Registro d;
	Registro c;
	char p;
	sprintf(a.nome, "Joao Gabriel Melo Barbirato");
	sprintf(a.cpf, "06460532486");
	a.idade = 21;

	sprintf(b.nome, "Gianna Melo Barbirato");
	sprintf(b.cpf, "41193784468");
	b.idade = 55;

	sprintf(d.nome, "Gabriela Purini Chermont");
	sprintf(d.cpf, "43810760870");
	d.idade = 20;

	for(int i=0; i < 7; i++){
		arqDados.insere(a);
	}
	arqDados.insere(b);
	arqDados.insere(b);
	arqDados.insere(d);
	//arqDados.lista();
	arqDados.busca(d.cpf,c);

	cout << c.cpf << endl;
	cout << c.nome << endl;
	cout << c.idade << endl;

	//cout << b.cabecalho.topoRegistros << endl;
	//cout << b.cabecalho.nRegistros << endl;
	//cout << b.cabecalho.inicio << endl;
	//cout << b.fimBloco << endl;
	
	//	FILE * teste = fopen("oimundo", "rb");
	//fseek(teste, TAM_BLOCO + 2*TAM_REG, SEEK_SET );
	//fread(&c, TAM_REG, 1, teste);
	//fclose(teste);
/*
	fread(&p, sizeof(char), 1, teste);
	cout << p << endl;
	fseek(teste, TAM_REG_CABECALHO, SEEK_SET);
	if(arqDados.busca("41193784468", c))
		cout << c.nome << endl;
*/
	return EXIT_SUCCESS;
}