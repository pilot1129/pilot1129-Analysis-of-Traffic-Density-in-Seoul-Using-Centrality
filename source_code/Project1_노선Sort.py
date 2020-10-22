#_*_coding:utf-8_*_
import numpy as np
import csv
import codecs
import operator
import pandas as pd
import pandas

bRoute = pd.read_csv('수정파일_수정.csv', names=['busnumber', 'IDnumber'], encoding='utf-8')

rate = pd.read_csv('토평공비율_수정.csv', names=['busnumber', 'IDnumber', '토승차', '토하차', '평승차', '평하차', '공승차', '공하차'], encoding ='utf-8')
a = bRoute.groupby([bRoute['busnumber'],bRoute['IDnumber']], sort=False).sum()
b = rate.groupby([rate['busnumber'],rate['IDnumber']], sort=False).sum()
#b = time.groupby([time['busnumber'], time['IDnumber']], sort=False).sum()
#result = pd.concat([a, b], axis=1, join='outer', sort=False)
result = pd.merge(bRoute,rate,how='left',on=['busnumber','IDnumber'])
result.to_csv("sorting_비율.csv",header = False, index = False, encoding = 'euc_kr')
print(result)