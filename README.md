# CircuitSim - Simulador de Circuitos Lógicos
> Um simulador de circuitos lógicos digitais com interface gráfica e roteamento de fios inteligente, desenvolvido em C++/Qt como projeto final da disciplina de Linguagem de Programação.

## 📜 Sobre o Projeto

O CircuitSim é uma aplicação de desktop que permite aos usuários construir e simular circuitos lógicos digitais de forma visual e interativa. O projeto foi concebido para aplicar e demonstrar conceitos avançados de programação orientada a objetos, design de interfaces gráficas e, principalmente, a implementação de algoritmos de busca de caminho para resolver problemas práticos de roteamento.

## ✨ Funcionalidades Principais

* **Interface Gráfica Intuitiva:** Construída com o framework Qt, a interface possui um tema escuro customizado, uma caixa de ferramentas (`QToolBox`) para seleção de componentes e uma área de desenho principal (canvas) com grade para facilitar o alinhamento.
* **Simulação em Tempo Real:** O estado lógico do circuito é propagado e atualizado instantaneamente. Ao alternar o estado de uma entrada, o resultado é visto imediatamente nas saídas e conexões, sem a necessidade de um botão "Simular".
* **Roteamento de Fios com A\*:** A funcionalidade de destaque do projeto. As conexões entre os componentes não são apenas linhas retas; elas são calculadas usando uma implementação do **algoritmo A* (A-Estrela)**, que:
    * Encontra o caminho mais curto entre dois pontos.
    * **Desvia automaticamente** de outros componentes, tratando-os como obstáculos.
    * Recalcula todas as rotas afetadas quando um componente é movido.
    * Garante que os fios saiam e cheguem nos componentes com segmentos retos, para um visual mais limpo.
* **Manipulação Direta:**
    * **Adicionar:** Arraste componentes da caixa de ferramentas para o canvas.
    * **Mover:** Clique e arraste qualquer componente pelo canvas.
    * **Conectar:** Crie conexões arrastando o mouse de um pino de saída para um pino de entrada válido.
    * **Deletar:** Remova componentes ou conexões com a ferramenta "Lixeira".
* **Persistência:** O estado completo do circuito (componentes, posições e conexões) pode ser salvo em um arquivo com formato customizado (`.csim`) e carregado posteriormente, permitindo a continuidade do trabalho.

## 🛠️ Tecnologias e Conceitos Aplicados

* **Linguagem:** C++
* **Framework:** Qt 6 (Widgets, Painter, Signals & Slots, File I/O)
* **Arquitetura:**
    * **Programação Orientada a Objetos:** Uso intensivo de herança (hierarquia de `Componente`) e polimorfismo (nas funções `desenhar()` e `calcularSaida()`).
    * **Separação de Responsabilidades:** Divisão clara entre a lógica do circuito (Core) e a representação gráfica (GUI).
* **Algoritmo Principal:**
    * **A\* (A-Estrela):** Implementado do zero para a busca de caminhos em uma grade 2D com obstáculos.
* **Estruturas de Dados:** `std::vector` e `std::map` para gerenciamento dos elementos do circuito e nós do algoritmo A*.
* **Estilização:** QSS (Qt Style Sheets) para a criação do tema escuro e customização da aparência dos widgets.

## 🚀 Como Compilar e Executar

1.  **Pré-requisitos:**
    * Qt Creator com um compilador C++ configurado (MinGW, MSVC ou Clang).
    * Framework Qt 6 instalado.
2.  **Passos:**
    * Clone este repositório: `git clone github.com/felipeschmitt04/circuitsim`
    * Abra o arquivo de projeto `CircuitSim.pro` com o Qt Creator.
    * Configure o "Kit" de compilação, se solicitado.
    * Compile e execute o projeto (botão "Play" ou `Ctrl+R`).

## 👨‍💻 Desenvolvedor

* **Nome:** Felipe Luan Schmitt
* **Contato:** felipe.lschmitt04@gmail.com
* **GitHub:** [github.com/felipeschmitt04](https://github.com/felipeschmitt04)
