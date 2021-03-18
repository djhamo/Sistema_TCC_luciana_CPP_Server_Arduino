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

% a = [1 2 3 4 5]
% b = [1 3 5 7 9 11]
% c = ismember(a,b)
% Lc = length(c)
% for i = 1:Lc
%     if ~c(i)
%         b = [b, a(i)]
%     end
% end

c = ismember(tPB, t);
Lc = length(c);
for i = 1:Lc
    if ~c(i)
        t = [t, tPB(i)];
    end
end

c = ismember(tQA, t);
Lc = length(c);
for i = 1:Lc
    if ~c(i)
        t = [t, tQA(i)];
    end
end

c = ismember(tQB, t);
Lc = length(c);
for i = 1:Lc
    if ~c(i)
        t = [t, tQB(i)];
    end
end

clear c i Lc

% ordenar t
t = sort(t);

tabelaDeltas = zeros(length(t), 5);
tabelaDeltas = [t', tabelaDeltas];

[aa, a] = ismember(tPA, t);
[bb, b] = ismember(tPB, t);
[cc, c] = ismember(tQA, t);
[dd, d] = ismember(tQB, t);

clear aa bb cc dd

L = length(a);
for i = 1:L
    ta = find(t == tPA(i));
    va = PA.v(i);
    tabelaDeltas(ta, 2) = va;
end

L = length(b);
for i = 1:L
    tb = find(t == tPB(i));
    vb = PB.v(i);
    tabelaDeltas(tb, 3) = vb;
end

L = length(c);
for i = 1:L
    tc = find(t == tQA(i));
    vc = QA.v(i);
    tabelaDeltas(tc, 4) = vc;
end

L = length(d);
for i = 1:L
    td = find(t == tQB(i));
    vd = QB.v(i);
    tabelaDeltas(td, 5) = vd;
end

clear L a b c d ta tb tc td va vb vc vd

L = length(tabelaDeltas);
for i = 1:L
    if tabelaDeltas(i, 2) ~= 0
        if tabelaDeltas(i, 3) ~=0
            tabelaDeltas(i, 6) = 1;
        end
    end
end

tabelaDeltas

% figure
% box on
% stem(tabelaDeltas(:,1)/3600,tabelaDeltas(:,2), tabelaDeltas(:,3))
% title("Tabela de Eventos")
% xlabel("Tempo [h]")
% ylabel("Eventos")
% legend("PA [W]", "PB [W]", "QA [var]", "QB [var]")
% axis([0 24 -4000 4000])

