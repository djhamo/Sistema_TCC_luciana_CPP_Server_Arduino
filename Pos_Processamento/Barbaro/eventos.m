%clc
%close all
%clear all

%load("20100418.mat");

%d = IArmsv(1:5000);
%d = QAtoN(1:5000);
%dadosLeitura = PAtoN();

mod = [];
tVar = [];
booMudou = false; %Valor Mudou?
booEstabilizou = false; %Valor Estabilizou?
booRuido = false; %Era Ruido?

valorPassado = 0; %Valor Passado
valorAtual = 0; %Valor Atual
valorAuxiliar = 0; %Valor Auxiliar
tempoInstantaneo = 0; %Valor do tempo

% P
%tolerancia1 = 20;
%tolerancia2 = 30;
%tolerancia3 = 5;

% Q
%tolerancia1 = 10;
%tolerancia2 = 15;
%tolerancia3 = 3;

valorAtual = sum(dadosLeitura(1:5))/5;
tVar = [1];
mod = [valorAtual];
var = [valorAtual-valorPassado];
contador1 = 11; %Contador 1
while contador1 <= (size(dadosLeitura, 2))
    %Atualizar Variáveis
    valorPassado = valorAtual;
    valorAtual = dadosLeitura(contador1);
    
    %Verificar alteração na leitura
    if valorAtual > valorPassado+tolerancia1 | valorAtual < valorPassado-tolerancia1
        booMudou = true;
        
        %Calculando a média antes (mediaAntes) e
        %depois (mediaDepois) da variação de leitura
        n = 10;
        mediaAntes = sum(dadosLeitura(contador1-n:contador1-1))/n;
        mediaDepois = sum(dadosLeitura(contador1+1:contador1+n))/n;
        
        %verificar se foi ruido
        if mediaDepois <= mediaAntes+tolerancia2 & mediaDepois >= mediaAntes-tolerancia2
            booRuido = true;
        else
            booRuido = false;
            tempoInstantaneo = contador1;
        end
    else
        booMudou = false;
    end
    
    %Se a leitura mudar, e não for
    %ruido, calcular novo valor
    if booMudou & ~booRuido
        %Processar enquanto novo valor
        %não estabilizar
        booEstabilizou = false;
        while ~booEstabilizou
            contador1 = contador1+1;
            valorAuxiliar = valorAtual;
            valorAtual = dadosLeitura(contador1);
            
            %Verificar se estabilizou
            if valorAtual <= valorAuxiliar+tolerancia3 & valorAtual >= valorAuxiliar-tolerancia3
                booEstabilizou = true;
                
                %Salvar alteração quando estabilizar
                mod = [mod valorAtual];
                var = [var valorAtual-valorPassado];
                tVar = [tVar tempoInstantaneo];
            else
                booEstabilizou = false;
            end
        end
        
    end
    
    contador1 = contador1+1;
end

% figure(1)
% hold on
% plot(dadosLeitura)
% stem(tVar, mod)