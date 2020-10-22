#_*_coding:utf-8_*_
import numpy as np
import csv
import codecs
import operator
import pandas as pd
import pandas
#요일별 승하차량 파일받음
dfSat = pandas.read_csv('1월 토요일.csv', names =['busnumber', 'arsnumber', '승차량', '하차량'])
dfHol = pandas.read_csv('1월 공휴일.csv', names =['busnumber', 'arsnumber', '승차량', '하차량'])
dfWeek = pandas.read_csv('1월 평일.csv', names =['busnumber', 'arsnumber', '승차량', '하차량'])
"""
비율계산을 위해 위 3라인의 승차량 하차량부분을 구하고자 하는 비율 이름으로 바꿔줘야함
"""

#각 날짜별 비율을 구함
dfMonth = [dfSat, dfHol, dfWeek]
dfResult = pd.concat(dfMonth, ignore_index=True)
Result = dfResult.groupby([dfResult['busnumber'] ,dfResult['arsnumber']]).sum()
ResultSat = dfSat.groupby([dfSat['busnumber'] ,dfSat['arsnumber']]).sum()
ResultHol = dfHol.groupby([dfHol['busnumber'] ,dfHol['arsnumber']]).sum()
ResultWeek = dfWeek.groupby([dfWeek['busnumber'] ,dfWeek['arsnumber']]).sum()
RateSat = ((ResultSat/Result)/4).reset_index()
RateWeek = ((ResultWeek/Result)/20).reset_index()
RateHol = ((ResultHol/Result)/4).reset_index()
RateSat=RateSat.rename(columns = {'승차량':'토요일승차비율'})
RateSat=RateSat.rename(columns = {'하차량':'토요일하차비율'})
RateWeek=RateWeek.rename(columns = {'승차량':'평일승차비율'})
RateWeek=RateWeek.rename(columns = {'하차량':'평일하차비율'})
RateHol=RateHol.rename(columns = {'승차량':'공휴일승차비율'})
RateHol=RateHol.rename(columns = {'하차량':'공휴일하차비율'})
Ratesum = pd.merge(RateSat, RateHol, on=['busnumber','arsnumber'])
Ratesum = pd.merge(Ratesum, RateWeek, on=['busnumber', 'arsnumber'])
Ratesum.to_csv("토평공비율.csv",header = False, index = False, encoding = 'euc_kr') #비율 파일 생성

#time = pd.read_csv('서울시 버스노선별 정류장별 시간대별 승하차 인원 정보.csv', names= ['busnumber', 'arsnumber', '00시승차', '00시하차', '01시승차', '01시하차', '02시승차', '02시하차', '03시승차', '03시하차', '04시승차', '04시하차', '05시승차', '05시하차', '06시승차', '06시하차', '07시승차', '07시하차', '08시승차', '08시하차', '09시승차', '09시하차', '10시승차', '10시하차', '11시승차', '11시하차', '12시승차', '12시하차', '13시승차', '13시하차', '14시승차', '14시하차', '15시승차', '15시하차', '16시승차', '16시하차', '17시승차', '17시하차', '18시승차', '18시하차', '19시승차', '19시하차', '20시승차', '20시하차', '21시승차', '21시하차', '22시승차', '22시하차', '23시승차', '23시하차'], encoding ='utf-8')
#timegroup = time.groupby([time['busnumber'], time['arsnumber']]).sum().reset_index()#[['00시승차', '00시하차']].sum()#time을 그룹화

"""
for (k1,k2), group in timegr['busnumber', 'arsnumber']:
	print(k1,k2)
	print(group)
"""