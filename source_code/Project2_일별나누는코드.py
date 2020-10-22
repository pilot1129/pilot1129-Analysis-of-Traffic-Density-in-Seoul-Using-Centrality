import numpy as np
import csv
import codecs
import pandas as pd

busList = []
inList = []
outList = []
idList = []#ars
f = open('1월.csv', 'r', encoding='utf-8')
csvReader = csv.reader(f)
for i in csvReader:
	a = str(i[0])
	if(a == '20190101'):
		busList.append(i[2])
		inList.append(i[8])
		outList.append(i[9])
		idList.append(i[4])
f.close()
lst = [busList, idList, inList, outList]
c = np.array(lst)
np.transpose(c)
df = pd.DataFrame(np.transpose(c))
df.to_csv("0101.csv",header = False, index = False)