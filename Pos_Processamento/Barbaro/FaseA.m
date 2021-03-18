clc
close all
clear all

load("20100418.mat");

dadosLeitura = PAtoN();
tolerancia1 = 20;
tolerancia2 = 30;
tolerancia3 = 5;
eventos

PA.m = mod;
PA.v = var;
PA.t = tVar;
save("PAdados.mat", "PA")

dadosLeitura = QAtoN();
eventos

QA.m = mod;
QA.v = var;
QA.t = tVar;
save("QAdados.mat", "QA")

deltas

%dryer=tabelaDeltas(abs(tabelaDeltas(:, 2)) > 2000, :)