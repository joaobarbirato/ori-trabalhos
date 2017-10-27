TAM_REGISTRO = 100

def listagem():
	#abre arquivos de teste para leitura
	arquivo_de_dados = open('arquivo_de_dados.txt', 'r') #abre o arquivo de dados para leitura
	arquivo_indice = open('arquivo_indice.txt', 'r') #abre o arquivo de dados para leitura

	conteudo = arquivo_de_dados.readlines()
	print(conteudo)

	#fecha arquivos de teste
	arquivo_de_dados.close #fecha arquivo de dados
	arquivo_indice.close #fecha arquivo de dados
