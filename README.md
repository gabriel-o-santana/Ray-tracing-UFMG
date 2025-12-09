# Ray Tracing


## Descrição do Projeto

Este projeto consiste na implementação de um renderizador baseado na técnica de **Ray Tracing**, desenvolvido em C++. O software é capaz de ler arquivos de descrição de cena e gerar imagens realistas simulando o transporte de luz.

O projeto atende aos requisitos da especificação do trabalho, suportando iluminação global, sombras, materiais procedurais, texturas e múltiplas primitivas geométricas.

### Funcionalidades Implementadas

* **Câmera:**
    * Posicionamento arbitrário (Eye, At, Up).
    * Controle de abertura vertical (FOV).
* **Iluminação (Modelo de Phong):**
    * **Luz Ambiente:** A primeira luz do arquivo (índice 0) é tratada estritamente como luz ambiente global.
    * **Luzes Pontuais:** Suporte a múltiplas fontes de luz com atenuação constante, linear e quadrática.
    * **Sombras:** Cálculo de sombras projetadas (Shadow Rays).
* **Materiais (Pigmentos):**
    * `Solid`: Cor sólida RGB.
    * `Checker`: Textura procedural 3D (tabuleiro de xadrez).
    * `Texmap`: Mapeamento de textura utilizando imagens em formato PPM.
* **Acabamentos (Propriedades Ópticas):**
    * Coeficientes: Ambiente ($k_a$), Difuso ($k_d$) e Especular ($k_s$).
    * Brilho: Expoente de rugosidade ($\alpha$).
    * **Reflexão:** Recursiva perfeita ($k_r$).
    * **Refração:** Transparência com índice de refração e Lei de Snell ($k_t$, $ior$).
* **Primitivas Geométricas:**
    * **Esferas:** Interseção analítica quadrática.
    * **Poliedros Convexos:** Interseção calculada via interseção de semi-espaços (planos).

---

## Estrutura do Projeto

* `src/`: Código fonte (`.cpp`).
* `include/`: Arquivos de cabeçalho (`.hpp`).
* `files/`: Contém arquivos de entrada de exemplo e as respectivas imagens de referência geradas.
* `CMakeLists.txt`: Configuração de build via CMake.

---

## Instruções de Compilação

### **Pré-requisitos:**
* Compilador C++ (g++, clang ou MSVC) com suporte a C++17.
* CMake (versão 3.10 ou superior).

1.  Abra o terminal na raiz do projeto.
2.  Crie e entre no diretório de build:
    ```bash
    mkdir build
    cd build
    ```
3.  Gere os arquivos de compilação:
    ```bash
    cmake ..
    ```
4.  Compile o executável:
    * **Linux / macOS:**
        ```bash
        make
        ```
    * **Windows:**
        ```bash
        cmake --build .
        ```

O executável `raytracer` (ou `raytracer.exe`) será criado dentro da pasta `build`.

---

## Instruções de Execução

O programa deve ser executado via linha de comando a partir da pasta onde o executável foi criado (`build/`).

**Sintaxe:**
```bash
./raytracer <arquivo_entrada> <arquivo_saida.ppm> [largura] [altura]