TAM_REGISTRO = 100

def insercao(lf, reg):
    # verificar se há espaços vazios
    topo = lf.getHeader().pop() #desempilha para verificação
    if(lf.getHeader().getTopo()>-1): #se há, pegue o primeiro da lista invertida ( pilha)
        lf.seek(TAM_REGISTRO * topo + lf.getHeader().getSize(), 0) # relativo ao começo do arquivo
        lf.write(reg)
    else: # se não há espaços vazios
        lf.getHeader().push(topo) #empilha para restauração
        lf.seek(0, 2) # relativo ao último registro do arquivo de dados
        lf.write(reg)
