# OpenMP
написать на C++ программу программу, реализующую многопоточность 
на основе технологии OpenMP: она должна вычислять значение функциис помощью редукции. 
Измерять время работы программ для различных  значений параметра N. 

файл l3.cpp
На основе двух равно размерных массивов A и B (длины N) функция возвращает сумму значений Max(Ai+Bi, 4Ai-Bi), больших 1.

файл l4.cpp
Модифицировать программу, используя синхронизацию вместо редукции. 

файл l5.cpp 
Модифицировать программы, распределив вычислительную нагрузку по секциям 
следующим образом: 
1)разбиваем вычисления на 2 секции – в каждую секцию отправляем половину строк исходных матриц для вычисления половины строк результирующей матрицы;
2)разбиваем вычисления на 4 секции – в каждую секцию отправляем четверть строк исходных матриц для вычисления четверти строк результирующей матрицы.

файл l6.cpp 
Модифицировать программы, используя переменные семафорного типа.
Модифицировать программы, используя барьерную синхронизацию и при необходимости nowait. 