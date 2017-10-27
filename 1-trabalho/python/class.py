import sys
class Registro:
    pass

class RegistroIndice:
    nome=''
    pass

class ArquivoIndice:
    def __init__(self, nomeArquivoIndice, tipoArquivo):
        self.arquivo_indice = open(nomeArquivoIndice, tipoArquivo)

    def __del__(self):
        pass

class Arquivo:
    def __init__(self):
        self.indice = ArquivoIndice.ArquivoIndice("arquivo_indice", "wb")
        arquivo_de_dados = open("arquivo_de_dados", "wb")
        aqruivo_de_dados.write(-1)
        arquivo_de_dados = close()

inteiro = '230796 Joao Gabriel melo barbirato'
print(sys.getsizeof(inteiro))