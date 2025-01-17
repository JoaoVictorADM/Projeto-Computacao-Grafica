# Animação Gráfica com C++, OpenGL, CUDA, GLFW e GLEW

Este projeto recria parte de uma animação da Disney utilizando C++, OpenGL, CUDA, GLFW e GLEW. A principal aplicação do projeto é a criação de animações por meio de computação gráfica avançada, utilizando técnicas como transformação geométrica e shaders para a renderização de objetos 3D. A CUDA foi utilizada para escrever os shaders.

## Tecnologias Utilizadas

- **C++**: Linguagem de programação principal utilizada para a implementação do projeto. Garantiu alta performance no processamento e manipulação dos gráficos.
- **OpenGL**: Biblioteca gráfica utilizada para renderização 3D dos objetos na cena. Responsável pela criação de objetos e pelo gerenciamento das transformações geométricas.
- **CUDA**: Framework de computação paralela usado para escrever e otimizar os shaders. A CUDA permite acelerar o processo de geração e execução dos shaders, tornando a animação mais eficiente.
- **GLFW**: Biblioteca para criar e gerenciar janelas interativas, além de fornecer suporte para a entrada de usuário (como teclado e mouse).
- **GLEW**: Biblioteca que facilita o acesso a funções avançadas do OpenGL, proporcionando compatibilidade com versões mais recentes da API.

## Funcionalidades

### Criação de Objetos Gráficos

O projeto inclui a implementação de classes para representar objetos fundamentais no contexto 3D, tais como:

- **Linha**: Utilizada para desenhar segmentos de reta, elementos básicos na construção de formas e estruturas gráficas.
- **Triângulo**: Representa uma das formas primárias em gráficos 3D, essencial para a composição de superfícies e objetos.

Essas classes são combinadas para desenhar objetos na cena e realizar animações dinâmicas por meio de transformações geométricas.

### Transformações Geométricas

As transformações geométricas são aplicadas aos objetos gráficos para criar animações realistas. As principais transformações incluem:

- **Translação**: Movimentação dos objetos no espaço tridimensional ao longo dos eixos X, Y e Z.
- **Rotação**: Rotacionamento dos objetos ao redor de eixos específicos, ajustando a orientação dos mesmos no espaço.
- **Escala**: Modificação do tamanho dos objetos para criar efeitos de zoom e distorções durante a animação.

Essas transformações são fundamentais para gerar a movimentação e interação dos objetos na cena.

### Shaders (Vertex e Fragment) Escritos com CUDA

A principal inovação do projeto é o uso da **CUDA para escrever os shaders**. Em vez de depender de implementações tradicionais de shaders em OpenGL, a CUDA foi utilizada para criar shaders personalizados que processam de forma otimizada:

- **Vertex Shader**: Manipula os vértices dos objetos gráficos, aplicando as transformações geométricas necessárias para a renderização 3D.
- **Fragment Shader**: Determina a cor e as características visuais de cada pixel da cena renderizada, aplicando efeitos como iluminação, sombras e texturização.

