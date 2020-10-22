import numpy as np
import csv
import codecs
import operator
import pandas as pd
import pandas



dfSat = pandas.read_csv('1월 토요일.csv', names =['busnumber', 'IDnumber', '승차량', '하차량'])
dfHol = pandas.read_csv('1월 공휴일.csv', names =['busnumber', 'IDnumber', '승차량', '하차량'])
dfWeek = pandas.read_csv('1월 평일.csv', names =['busnumber', 'IDnumber', '승차량', '하차량'])
dfMonth = [dfSat, dfHol, dfWeek]#3개 파일 합쳐줌
dfResult = pd.concat(dfMonth, ignore_index=True)#3개 파일 합쳐줌
Result = dfResult.groupby([dfResult['busnumber'] ,dfResult['IDnumber']]).sum()#총 합
ResultSat = dfSat.groupby([dfSat['busnumber'] ,dfSat['IDnumber']]).sum()#토요일 합
ResultHol = dfHol.groupby([dfHol['busnumber'] ,dfHol['IDnumber']]).sum()#공휴일 합
ResultWeek = dfWeek.groupby([dfWeek['busnumber'] ,dfWeek['IDnumber']]).sum()#평일 합
RateSat =(ResultSat/Result)/4
RateWeek =(ResultWeek/Result)/22
RateHol =(ResultHol/Result)/5
a= pd.merge(RateSat,RateWeek, how="outer", left_index=True, right_index=True)
b = pd.merge(a, RateHol, how="outer", left_index=True, right_index=True)
b.reset_index().to_csv("토평공비율(1일당).csv",header = False, index = False, encoding = 'euc_kr')
print(b)
"""
for (k1,k2), group in 그룹이름:
	print(k1,k2)
	print(group)
"""#그룹화가 잘 되었는지 확인
#print(ResultSat/Result + ResultHol/Result + ResultWeek/Result) # 값이 1로 나오면 식이 맞다는 증거
