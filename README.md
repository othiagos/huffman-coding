# Huffman Coding

**Descrição em alto nível da funcionalidade do código:**
>O projeto é desenvolvido para compactar arquivos de texto codificados em UTF-8. Ele utiliza o algoritmo de Huffman para realizar a compactação do arquivo, com foco em nível de letra.<br/>
O algoritmo de Huffman é uma técnica de compressão que atribui códigos de tamanho variável para cada letra no arquivo, com base na frequência de ocorrência de cada letra. Letras mais frequentes recebem códigos mais curtos, enquanto letras menos frequentes recebem códigos mais longos. Isso permite uma representação mais eficiente das letras no arquivo compactado.<br/>
Ao executar o programa, o usuário fornece o caminho do arquivo de texto a ser compactado. O programa lê o arquivo, analisa a frequência de cada letra e constrói uma árvore de Huffman com base nessa análise. Em seguida, os códigos de compactação são gerados a partir da árvore de Huffman.

## **Compilação**

```bash
$ make

```
## **Execução**
```bash
$ make run ARGS="FLAGS"
```

## **Instruções para execução**
```
[OPTION] [FILE] [FILE]

-c <arquivo_entrada> <arquivo_saida> compacta o arquivo entrada no arquivo saída  
-d <arquivo_entrada> <arquivo_saida> descompacta o arquivo entrada no arquivo saída  
```
## **Execução Testes**

```bash
$ make run_test
```

## **Arquivos disponíveis**
Arquivo em UTF-8 para demonstrar a o funcionamento
```
in/msg.txt
```

