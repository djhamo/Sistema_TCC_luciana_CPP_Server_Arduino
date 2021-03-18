%clc
%close all
%clear all

load("PAdados.mat");
load("PBdados.mat");
load("QAdados.mat");
load("QBdados.mat");

tPA = PA.t;
tPB = PB.t;
tQA = QA.t;
tQB = QB.t;

t = tPA;

c = ismember(tQA, t);
Lc = length(c);
for i = 1:Lc
    if ~c(i)
        t = [t, tQA(i)];
    end
end

clear c i Lc

% ordenar t
t = sort(t);

tabelaDeltasA = zeros(length(t), 5);
tabelaDeltasA = [t', tabelaDeltasA];

[aa, a] = ismember(tPA, t);
[bb, b] = ismember(tPB, t);
[cc, c] = ismember(tQA, t);
[dd, d] = ismember(tQB, t);

clear aa bb cc dd

L = length(a);
for i = 1:L
    ta = find(t == tPA(i));
    va = PA.v(i);
    tabelaDeltasA(ta, 2) = va;
end

L = length(b);
for i = 1:L
    tb = find(t == tPB(i));
    vb = PB.v(i);
    tabelaDeltasA(tb, 3) = vb;
end

L = length(c);
for i = 1:L
    tc = find(t == tQA(i));
    vc = QA.v(i);
    tabelaDeltasA(tc, 4) = va;
end

L = length(d);
for i = 1:L
    td = find(t == tQB(i));
    vd = QB.v(i);
    tabelaDeltasA(td, 5) = vd;
end

clear L a b c d ta tb tc td va vb vc vd

L = length(tabelaDeltasA);
for i = 1:L
    if tabelaDeltasA(i, 2) ~= 0
        if tabelaDeltasA(i, 3) ~=0
            tabelaDeltasA(i, 6) = 1;
        end
    end
end

tabelaDeltasA

figure(2)
box on
stem(tabelaDeltasA(:,1)/3600,tabelaDeltasA(:,2:5))
title("Tabela de Eventos")
xlabel("Tempo [h]")
ylabel("Eventos")
legend("PA [W]", "PB [W]", "QA [var]", "QB [var]")
axis([0 24 -4000 4000])

