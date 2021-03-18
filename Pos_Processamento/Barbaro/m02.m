clc
close all
clear all

load("20100418.mat");

%d = IArmsv(1:5000);
%d = QAtoN(1:5000);
d = QAtoN();

mod = [];
tVar = [];
mudou = false; %Valor Mudou?
est = false; %Valor Estabilizou?
ruido = false; %Era Ruido?
t1 = 0.1; %Tolernância 1
t2 = 0.2; %Tolernância 2
t3 = 0.03; %Tolernância 3
vP = 0; %Valor Passado
vA = 0; %Valor Atual
vX = 0; %Valor auXiliar
vT = 0; %Valor do tempo

% P
%t1 = 20;
%t2 = 30;
%t3 = 5;

% Q
t1 = 10;
t2 = 15;
t3 = 3;

vA = sum(d(1:5))/5;
tVar = [1];
mod = [vA];
var = [vA-vP];
c1 = 1; %Contador 1
while c1 <= (size(d, 2))
    %Atualizar Variáveis
    vP = vA;
    vA = d(c1);
    
    %Verificar alteração na leitura
    if vA > vP+t1 | vA < vP-t1
        mudou = true;
        
        %Calculando a média antes (m1) e
        %depois (m2) da variação de leitura
        n = 5;
        m1 = sum(d(c1-n:c1-1))/n;
        m2 = sum(d(c1+1:c1+n))/n;
        
        %verificar se foi ruido
        if m2 <= m1+t2 & m2 >= m1-t2
            ruido = true;
        else
            ruido = false;
            vT = c1;
        end
    else
        mudou = false;
    end
    
    %Se a leitura mudar, e não for
    %ruido, calcular novo valor
    if mudou & ~ruido
        %Processar enquanto novo valor
        %não estabilizar
        est = false;
        while ~est
            c1 = c1+1;
            vX = vA;
            vA = d(c1);
            
            %Verificar se estabilizou
            if vA <= vX+t3 & vA >= vX-t3
                est = true;
                
                %Salvar alteração quando estabilizar
                mod = [mod vA];
                var = [var vA-vP];
                tVar = [tVar vT];
            else
                est = false;
            end
        end
        
    end
    
    c1 = c1+1;
end

figure(1)
hold on
plot(d)
stem(tVar, mod)