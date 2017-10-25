TAM_REGISTRO = 100

def busca():
	#abre arquivos de teste para leitura
	arquivo_de_dados = open('arquivo_de_dados.txt', 'r') #abre o arquivo de dados para leitura
	arquivo_indice = open('arquivo_indice.txt', 'r') #abre o arquivo de dados para leitura

	#busca sequencial
	chave_busca = raw_input("Insira uma chave: ") #usuário informa a chave, deve ser um numero inteiro
	for linha in arquivo_indice:
		chave, RRN = linha.split(',') #quebra a string em duas strings utilizando a ',' como separador
		if(chave_busca == chave):
			RRN = int(RRN) #transforma a string RRN em um inteiro
			arquivo_de_dados.seek((TAM_REGISTRO*RRN),0) #posiciona para a posicao de inicio do registro encontrado
			registro = arquivo_de_dados.read(TAM_REGISTRO) #lê o registro
			print(registro) #mostra o registro encontrado na tela
			break #interrompe o loop e impede que outras ocorrencias de mesma chave sejam apresentadas

	#fecha arquivos de teste
	arquivo_de_dados.close #fecha arquivo de dados
	arquivo_indice.close #fecha arquivo de dados
