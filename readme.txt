Trabalho 1 de computação visual
Aluno: Rafael Henrique Tibaes


Requerimentos (Testado no Ubuntu 16.04):
- OpenCV (testado com a versão 3.1)
- GLFW (libglfw-dev)
- Glew (libglew-dev)
- GLM (libglm-dev)
- gcc (build-essentials)


Compilação:
  make clean all


Arquivos:
├── ctrlPts.hpp - Calculo da curva por casteljau e convolução circular
├── curveShader.frag - Fragment Shader para a renderização de edição 2D
├── curveShader.vert - Vertex Shader para a renderização de edição 2D
├── initShaders.cpp - Inicializador de Shader
├── initShaders.h - Cabeçalho do inicializador de Shader
├── loadTexture.hpp - Inicializador de textura
├── main.cpp - Main, controle de janela e delegação de renderizadores
├── Makefile - Regras de compilação
├── modelling.hpp - Controlador de renderização do modelo 3D
├── modelShader.frag - Fragment Shader para renderização do modelo 3D
├── modelShader.vert - Vertex Shader para renderização do modelo 3D
├── pattern.jpg - Arquivo de textura quadriculada utilizado na renderização 3D
├── preview.hpp - Controlador de renderização da curva 2D
├── readme.txt - este arquivo
└── RenderingProtocol.hpp - Protocolo para os controladores de renderização


Uso:
Ao iniciar a aplicação, uma janela com os eixos será apresentada. Com o clique
do mouse é possível adicionar um ponto de controle para a curva que define o
modelo. Caso seja necessário remover um ponto, a tecla 'R' desempilha o ponto de
controle no topo da pilha de pontos. Em tempo-real é renderizada a curva
obtida por Casteljau a partir do conjunto de pontos de controle.
Para vizualizar o objeto 3D, basta apertar a tecla '2' no teclado; e para voltar
ao modo de edição, a tecla '1'. A tecla 'Q' finaliza a aplicação. No modo de
renderização 3D, teclas '3' a '0' modificam o posicionamento da camera, contudo
esta etapa não está completa.


Problemas não solucionados:
1. é possível remover os pontos de controle, exceto o primeiro; caso seja
removido o primeiro ponto de controle, ocorrerá falha de segmentação.
2. não é possível reposicionar a camera; falhei ao projetar os calculos de
projeção da camera, e por este motivo deixei a visão de um ponto fixo.


Happy hacking!
