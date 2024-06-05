# TP Visão por Computador 

## Descrição do Projeto
Este trabalho tem como objetivo o desenvolvimento uma aplicação que, pela aplicação de um conjunto de
algoritmos, permita a contabilização e identificação de resistências elétricas presentes num vídeo. Esta aplicação
deverá ser capaz de segmentar as diferentes resistências (e apenas as resistências), identificando o seu valor
(expressando-o em ohm) e localização relativa (qual a região da imagem ocupada por cada resistência) em cada
frame do vídeo.
No vídeo estão presentes: 1 resistência de 5600Ω; 1 resistência de 220Ω; 2 resistências de 1000Ω; 1 resistência
de 2200Ω; e 1 resistência de 10000Ω.

## Dividimos este projeto em 3 partes

1. **Identificação das Resistências**
2. **Contagem de Blobs**
3. **Identificação da Capacidade das Resistências**

Para alcançar esses objetivos, utilizamos uma combinação de técnicas de processamento de imagem, segmentação por cores e análise de blobs. Abaixo, detalhamos cada uma das etapas envolvidas no processo.

## Estrutura do Projeto

### 1. Identificação das Resistências

1. **Conversão de RGB para HSV**:
    - O vídeo fornecido está em formato BGR, portanto, ajustamos a função de conversão para garantir a correta interpretação dos canais de cor.
    
2. **Segmentação HSV por Cor**:
    - Utilizamos a segmentação por cor para isolar as resistências. Este processo foi realizado por tentativa e erro, ajustando os valores do funil de cores HSV para obter uma segmentação o mais eficaz possivel.
    
3. **Dilatação dos Objetos**:
    - Aplicamos a função de dilatação do OpenCV para garantir que as resistências fossem identificadas corretamente como objetos de primeiro plano.

### 2. Contagem de Blobs

1. **Redução da Complexidade Temporal**:
    - Para reduzir a complexidade do tempo de processamento, limitamos a análise a uma faixa central da imagem (30% a 70% da largura da imagem), pois todas as resistências passam por essa região.

2. **Análise de Blobs**:
    - Filtramos os blobs com base no preenchimento da bounding box e no centro de massa, considerando suas dimensões.
    
3. **Função de Bounding box e centro de massa**:
    - A contagem e identificação das cores das resistências foram integradas na função bounding box para minimizar o processamento adicional.

4. **Contador de Resistências**:
    - Um contador é iniciado fora do loop do vídeo e é incrementado quando o centro de massa de uma resistência passa um limite definido no eixo x. Este limite foi ajustado para um intervalo que garante que todas as resistências sejam contadas apenas uma vez.

### 3. Identificação da Capacidade das Resistências

1. **Análise de Perfil do Blob**:
    - Para reduzir o processamento, analisamos apenas um perfil (linha) dentro do blob ao longo do eixo x.

2. **Conversão para HSV**:
    - Durante o processamento do bounding box e centro de massa, a imagem está em BGR. Criamos uma função para converter a posição atual para valores HSV, facilitando a detecção de cores.

3. **Armazenamento das Cores**:
    - Alteramos a estrutura IVC para adicionar quatro campos para armazenar as três cores e uma variável chamada `ultimaCor` para controlar a posição.

4. **Processamento das Cores**:
    - Iniciamos `ultimaCor` com -1. Ao iterar pela largura do blob, procuramos as cores definidas. Se uma cor é encontrada, `ultimaCor` é atualizada para essa cor, bloqueando novas procuras até que o amarelo (entre as cores da resistência) seja encontrado, momento em que `ultimaCor` é resetada para -1.

## Possíveis Perguntas

- **Como foram definidas as cores para a segmentação HSV?**
- **Qual o impacto da redução da área analisada na precisão do algoritmo?**
- **Como o limite para a contagem de resistências foi determinado?**
- **Quais são as cores específicas que o algoritmo procura identificar?**
- **Quais são os campos adicionados na estrutura IVC e qual a sua função?**
- **Como a dilatação dos objetos afeta a identificação das resistências?**
- **Quais técnicas de tentativa e erro foram usadas na segmentação por cor?**
- **Como a função de conversão para HSV foi implementada e ajustada?**
- **Qual é a precisão do algoritmo na contagem de resistências?**
- **Como foram escolhidas as cores específicas para segmentação e identificação?**
- **Quais desafios foram encontrados ao identificar a capacidade das resistências?**
- **Como a redução da área analisada impacta o desempenho do algoritmo?**
- **Quais ajustes foram feitos na estrutura IVC e por que eles são necessários?**
- **Como o algoritmo lida com diferentes velocidades das resistências no vídeo?**
- **Quais melhorias podem ser feitas no futuro para otimizar ainda mais o processamento?**
- **Como o projeto garante que cada resistência seja contada apenas uma vez?**
- **Quais métodos foram usados para validar a precisão do algoritmo?**
- **Como a segmentação por cor foi ajustada para diferentes condições de iluminação no vídeo?**
- **Quais são as limitações atuais do algoritmo e como elas podem ser superadas?**


