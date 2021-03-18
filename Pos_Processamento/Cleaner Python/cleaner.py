from scipy.io import savemat
import numpy as np
import sqlite3;

tensao = []
corrente = []

con = sqlite3.connect("test.db")
cur = con.cursor()

query = cur.execute('SELECT VALOR_TENSAO, VALOR_CORRENTE FROM LEITURAS;')
for row in query:
    _tensao = row[0]
    _corrente = row[1]
    if (type(_tensao) == int or type(_tensao) == float) and (type(_corrente) == int or type(_corrente) == float):
        #print(_tensao, _corrente)
        tensao.append(_tensao)
        corrente.append(_corrente)

con.close()

mdic = {"PAtoN": tensao, "PBtoN": corrente, "QA1toN": tensao, "QB1toN": corrente}
#print (mdic)
savemat("matlab_matrix.mat", mdic)