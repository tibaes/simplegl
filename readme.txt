Trabalho 1 de computação visual
Aluno: Rafael Henrique Tibaes

Requerimentos (Testado no Ubuntu 16.04):
- GLFW (libglfw-dev)
- Glew (libglew-dev)
- GLM (libglm-dev)
- gcc (build-essentials)

Compilação:
  make all

Arquivos:
├── .git - histórico de evolução do trabalho
├── ctrlPts.hpp - Calculo da curva por casteljau e convolução circular.
├── main.cpp - Renderização 2D da curva e 3D do modelo
├── Makefile - Compilação
├── readme.txt - Este arquivo
... shaders


Segue a lista de próximos passos:
1. Isolar o modo de preview 2D da curva, de sua vizualização em 3D.
2. Utilizar a biblioteca glm para o calculo de diferentes perspectivas e
  posicionamento da camera.
3. Realizar a renderização com iluminação e textura utilizando shaders.
4. Permitir a edição dos pontos de controle (remover, reiniciar)



Happy hacking!
