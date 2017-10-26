TAM_REGISTRO = 100
TAM_REG_INDICE = 50
# dado o registro
def remocao1(lf, reg):
    # verificar onde está no arquivo de índice
        # busca:
    indice = lf.getIndex()
    indice.seek(0, 0)
    achei = False
    while(indice.read(TAM_REG_INDICE) == None and !achei): # loop do arquivo de índice
        indice.seek(-TAM_REG_INDICE, 1) # em relação ao atual
        i = indice.read(TAM_REGISTRO)
        r = lf.read(TAM_REGISTRO * i.getRRN() + lf.getHeader().getSize(), 0) # em relação ao primeiro
        if(r == reg):
            achei = True
    # retorno
    if(!achei)
        print("Nao há esse registro.")
    else: # se achei o registro
        aux = lf.getHeader().pop()
        lf.write('@' + chr(aux))
        lf.getHeader().push(aux)
        lf.getHeader().push(i.getRRN()) # atualiza a lista invertida
        lf.seek(TAM_REGISTRO*i.getRRN() + lf.getHeader().getSize(), 0)
        return reg


# dado o RRN
def remocao2(lf, RRN):
    lf.seek(TAM_REGISTRO * RNN + lf.getHeader().getSize(), 0)
    lf.getHeader().push(RRN)
    return lf.read(TAM_REGISTRO)
