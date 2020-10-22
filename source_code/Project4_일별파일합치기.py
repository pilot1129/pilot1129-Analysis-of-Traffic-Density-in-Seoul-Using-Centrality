#_*_coding:utf-8_*_
import numpy as np
import csv
import codecs
import operator
import pandas as pd
import pandas



df1 = pandas.read_csv('0101.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df2 = pandas.read_csv('0102.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df3 = pandas.read_csv('0103.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df4 = pandas.read_csv('0104.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df5 = pandas.read_csv('0105.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df6 = pandas.read_csv('0106.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df7 = pandas.read_csv('0107.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df8 = pandas.read_csv('0108.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df9 = pandas.read_csv('0109.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df10 = pandas.read_csv('0110.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df11 = pandas.read_csv('0111.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df12 = pandas.read_csv('0112.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df13 = pandas.read_csv('0113.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df14 = pandas.read_csv('0114.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df15 = pandas.read_csv('0115.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df16 = pandas.read_csv('0116.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df17 = pandas.read_csv('0117.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df18 = pandas.read_csv('0118.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df19 = pandas.read_csv('0119.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df20 = pandas.read_csv('0120.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df21 = pandas.read_csv('0121.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df22 = pandas.read_csv('0122.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df23 = pandas.read_csv('0123.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df24 = pandas.read_csv('0124.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df25 = pandas.read_csv('0125.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df26 = pandas.read_csv('0126.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df27 = pandas.read_csv('0127.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df28 = pandas.read_csv('0128.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df29 = pandas.read_csv('0129.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df30 = pandas.read_csv('0130.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')
df31 = pandas.read_csv('0131.csv', names =['busnumber', 'idnumber', '승차량', '하차량'],encoding='utf-8')

weekday = [df2,df3,df4,df7,df8,df9,df10,df11,df14,df15,df16,df17,df18,df21,df22,df23,df24,df25,df28,df29,df30,df31]
sat = [df5,df12,df19,df26]
hol = [df1,df6,df13,df20,df27]
pd.concat(weekday).to_csv("1월 평일.csv",header = False, index = False)
pd.concat(sat).to_csv("1월 토요일.csv",header = False, index = False)
pd.concat(hol).to_csv("1월 공휴일.csv",header = False, index = False)