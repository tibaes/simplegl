Resultado parcial da implementação do trabalho 1 de computação visual
Aluno: Rafael Henrique Tibaes

Neste momento o resultado encontra-se bastante limitado. Somente pode ser
vizualizada a curva de casteljau com seus pontos de controle, e seu modelo
tridimensional com camera fixa.

Na área da esquerda, delimitada pelo perímetro de um quadrado, podem ser
adicionados os pontos de controle por meio do clique de mouse. A partir de dois
pontos de controle, a curva suavizada por casteljau é renderizada em tempo-real.
Também é renderizado na área da direita o modelo tridimensional formado pela
convolução circular da curva em torno do eixo y.

Segue a lista de próximos passos:
1. Isolar o modo de preview 2D da curva, de sua vizualização em 3D.
2. Utilizar a biblioteca glm para o calculo de diferentes perspectivas e
  posicionamento da camera.
3. Realizar a renderização com iluminação e textura utilizando shaders.
4. Permitir a edição dos pontos de controle (remover, reiniciar)

Arquivos:
├── .git - histórico de evolução do trabalho
├── ctrlPts.hpp - Calculo da curva por casteljau e convolução circular.
├── main.cpp - Renderização 2D da curva e 3D do modelo
├── Makefile - Compilação (não funciona no ubuntu, inversão da ordem do linker)
├── readme.txt - Este arquivo
├── shaders - Diretório com os shaders para uso futuro
│   ├── frag.shader - Shader de fragmentos
│   └── vert.shader - Shader de vertices
└── shaders.hpp - Carga dos arquivos de shader
.git

Compilação:
  g++ -std=c++11 -O3 main.cpp -o main -lGL -lGLEW -lglfw

Observação irrelevante: Segui o conselho de um colega de que glfw seria mais
fácil de manipular as janelas, e portanto utilizei um baseline com esta
biblioteca. Contudo, tenho minhas dúvidas quanto a produtividade a partir desta
decisão.

Happy hacking!
