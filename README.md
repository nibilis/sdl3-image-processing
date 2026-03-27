# Projeto 1 - Processamento de Imagens

## Sobre o Projeto


O objetivo do projeto é carregar imagens, realizar a conversão para escala de cinza, exibir um histograma a partir da imagem e aplicar uma equalização do mesmo utilizando por meio de uma GUI. O programa foi desenvolvido em C utilizando as bibliotecas SDL3 e SDL_image.

## Funcionalidades
* **Carregamento de Imagem:** Suporte a formatos comuns como PNG, JPG e BMP, com tratamento de erros para arquivos não encontrados ou inválidos.
* **Conversão para Escala de Cinza:** O programa detecta se a imagem esta em escala de cinza ou não, se não estiver, converte para escala de cinza utilizando a fórmula Y = 0.2125 * R + 0.7154 * G + 0.0721 * B.
* **Interface Gráfica (GUI) com duas janelas:** O sistema possui duas janelas integradas.
  * **Janela Principal:** Exibe a imagem carregada.
  * **Janela Secundária:** Janela contendo a exibição do histograma e os botões de interação.
* **Análise de Histograma:** Exibe informações sobre a média de intensidade e o contraste da imagem.
* **Equalização do Histograma:** Inclui um botão que aplica a equalização na imagem e no histograma, permitindo também reverter para a versão original ao ser clicado novamente.
* **Salvar Arquivo:** Pressionar a tecla S salva a imagem exibida como `output_image.png`, sobrescrevendo o arquivo caso ele já exista.

## Pré-requisitos e Compilação
* **Linguagem:**  C
* **Bibliotecas :** SDL3 e SDL_image
* **Compilador:** GCC

**Para compilar:** `gcc main.c -o main`

## Como Executar
O programa deve ser executado via linha de comando, passando o caminho da imagem como argumento.

**Comando:**
`./main caminho_da_imagem.ext`

* `main`: é o executável gerado após a compilação.
* `caminho_da_imagem.ext`: é o caminho para o arquivo que será processado.

## Equipe e Contribuições
- **Marco Antonio de Camargo, RA: 10418309:** Exibição do histograma e equalização de histograma e imagem.
- **Natan Moreira Passos, RA: 10417916:** Documentação e análise de histograma.
- **Nicolas Henriques de Almeida, RA: 10418357:** Carregamento de imagem, interface visual, detecção e aplicação de escala de cinza.

Obs: todos os membros do grupo prestaram suporte aos envolvidos em cada etapa e participaram de processos de debug.