clc
close all
clear all

load("20100418.mat");

%Fase A

dadosLeitura = PAtoN();

tolerancia1 = 20;
tolerancia2 = 30;
tolerancia3 = 5;

eventos

title("Fase A")
figure(1)
hold on
plot(dadosLeitura,'b')
stem(tVar, mod,'c')

PA.m = mod;
PA.v = var;
PA.t = tVar;
save("PAdados.mat", "PA")

dadosLeitura = QAtoN();

tolerancia1 = 10;
tolerancia2 = 15;
tolerancia3 = 3;

eventos

plot(dadosLeitura,'r')
stem(tVar, mod,'m')
hold off

QA.m = mod;
QA.v = var;
QA.t = tVar;
save("QAdados.mat", "QA")

%Fase B
dadosLeitura = PBtoN();

tolerancia1 = 20;
tolerancia2 = 30;
tolerancia3 = 5;

eventos

PB.m = mod;
PB.v = var;
PB.t = tVar;
save("PBdados.mat", "PB")

figure(2)
title("Fase B")
hold on
plot(dadosLeitura,'b')
stem(tVar, mod,'c')

dadosLeitura = QBtoN();

tolerancia1 = 10;
tolerancia2 = 15;
tolerancia3 = 3;

eventos

plot(dadosLeitura,'r')
stem(tVar, mod,'m')
hold off

QB.m = mod;
QB.v = var;
QB.t = tVar;
save("QBdados.mat", "QB")

deltas

figure
hold on
box on
plot(PAtoN,'b')
plot(QAtoN,'r')
stem(tabelaDeltas(:,1),tabelaDeltas(:,2), 'c')
stem(tabelaDeltas(:,1),tabelaDeltas(:,4), 'm')

title("Deltas - Fase A")
legend("PA [W]", "QA [var]", "DeltaPA", "DeltaQA")
hold off

figure
hold on
box on
plot(PBtoN,'b')
plot(QBtoN,'r')
stem(tabelaDeltas(:,1),tabelaDeltas(:,3), 'c')
stem(tabelaDeltas(:,1),tabelaDeltas(:,5), 'm')

title("Deltas - Fase B")
legend("PB [W]", "QB [var]", "DeltaPB", "DeltaQB")
hold off

figure
hold on
box on
plot(PAtoN,'b')
stem(tabelaDeltas(:,1),tabelaDeltas(:,2), 'r')


title("Detecção de Eventos")
legend("PA [W]", "Evento")
hold off

dryer=tabelaDeltas(abs(tabelaDeltas(:, 2)) > 2500, :)

filtroSecadora=tabelaDeltas(abs(tabelaDeltas(:, 2)) > 2500 & abs(tabelaDeltas(:, 6)) == 1, :)
filtroMicroondas=tabelaDeltas(abs(tabelaDeltas(:, 2)) > 1200 & abs(tabelaDeltas(:, 4)) > 300, :)
filtroGeladeira=tabelaDeltas(abs(tabelaDeltas(:, 3)) > 100 & abs(tabelaDeltas(:, 3)) < 300 & abs(tabelaDeltas(:, 5)) > 10, :)
filtroFreezer=tabelaDeltas(abs(tabelaDeltas(:, 2)) > 70 & abs(tabelaDeltas(:, 2)) < 200 & abs(tabelaDeltas(:, 4)) > 90, :)