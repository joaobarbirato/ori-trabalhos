# compactacao
def recursiva(lf, vPilha, elemento): #funcao auxiliar recursiva (pensar num nome melhor)
    lf.seek( (elemento + 1)*TAM_REGISTRO + lf.getHeader().getSize(), 0) # mova um pra direita
    if (elemento+1) is in (vPilha): # se o proximo à direita também é vazio
        foo(lf, vPilha, elemento+1)
    else: # se o próximo à direita não é vazio
        vPilha = vPilha.append(elemento+1)
        reg = lf.read(TAM_REGISTRO)
        lf.seek(2*TAM_REGISTRO, 1) # voltar à posição registro vazio
        lf.write(reg)

def compactacao(lf):
    topo = lf.getHeader().pop()
    if(top==-1): # se não há espaços vazios
        return "n ha espacos vazios"
    else:   # transfira a pilha para um vetor (mais maleavel)
        topo = vPilha[0]
        i=1
        while(topo != -1):
            vPilha[i] = lf.getHeader().pop()
            i=i+1

        for elemento in vPilha:
            recursiva(lf, vPilha, elemento)
        lf.getIndex().updateIndex() # depois de reorganizar o arquivo de dados, reorganizar o arquivo de indice
