## Intro
Trabalho para a matéria de automação de teste do CIN/UFPE, será um breve tutorial demonstrando uma simples demostração da
utilização da ferramenta KLEE.

## KLEE
Website: https://klee-se.org/
Descrição: KLEE is a dynamic symbolic execution engine built on top of the LLVM compiler infrastructure, and available under the UIUC open source license

## Dependências
Para a utilização do klee, é necessário utilizar um compilador para LLVM bitcode, a ferramente clang pode ser usado para isso: 
https://clang.llvm.org/ 

## Docker
O klee já disponibilizou uma imagem do docker com todas os itens necessários já instalado e configurado:
link tutorial usando docker: https://klee-se.org/docker/

### tutorial usando o arquivo teste.c desse git de exemplo
Para este exemplo, primeiramente inicia um container no docker com o seguinte comando:
```docker run --rm -ti --ulimit='stack=-1:-1' klee/klee:3.0```

No container do docker, crie uma pasta no /home/klee/cin
* 1º Criar pasta dentro do container: ```mkdir /home/klee/cin``` e vá até ela ```cd /home/klee/cin```
* 2º Na sua máquina, em um outro terminal, copie o arquivo do repositório com o comando ```docker cp teste.c container_id:/home/klee/cin/teste.c``` para importar o arquivo para o container.
Com isso, o arquivo teste.c estará dentro da pasta home/klee/cin do container.


## Passo a Passo
### Marcar imput como simbólico 
A função o qual está interessado a criar teste deve marcar os inputs simbólicos. Por exemplo, gerar testes para a função
calcula_valor(int a, int b, int c, int d) tem a seguinte marcação dos inputs simbólicos;
```
#include <klee/klee.h>

int calcula_valor(int a, int b, int c, int d) {
    ...
}

int main() {
  int a, b, c, d;
  klee_make_symbolic(&a, sizeof(a), "a"); //<----
  klee_make_symbolic(&b, sizeof(b), "b"); //<----
  klee_make_symbolic(&c, sizeof(c), "c"); //<----
  klee_make_symbolic(&d, sizeof(d), "d"); //<----

  int resultado = calcula_valor(a, b, c, d);

  return 0;
}
```

onde que o "a", o klee gerará testes com várias variáveis de "a" para tentar percorrer todas as branchs da função calcula_valor. Da mesma forma com os inputs b, c e d, simultâneamente.

No caso do nosso arquivo do repositório, já está tudo pronto!.

### Compilando o código para LLVM bitcode
usando clang, gerará o teste.bc, que é o código em formato de bitcode, o qual o klee interpreta para executar a suas
ferramentas.

Podendo executar o seguinte comando para que o klee consiga coletar mais informações, no nosso caso de exemplo no container,
executar:
```$ clang -I ~/klee_src/include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone teste.c```

ou o seguinte, para situações mais simples:
```$ clang -emit-llvm -c teste.c```

### Rodando Klee

Já com o programa klee instalado na máquina e reconhecido no terminal de comando (variável de path). No caso do container, já está tudo pronto, basta então executar ```$ klee --libc=uclibc --posix-runtime teste.bc```

com isso o klee gerará os testes para a pasta "klee-last". Nota que, nesse caso o klee irá reportar que o arquivo tem duas linhas
com erros críticos, ou seja, linhas que tem erros por divisão por 0. E provavelmente ele gerará os primeiros teste com os inputs
para acionar esse erro.

### Visualizar Klee teste

Para vizualisar os arquivos testes escrito em .ktest, utilizar o comando ```ktest-tool klee-last/test000001.ktest```, pois esta
extensão é um binário, com o ktest-tool, ele listará todos os valores de cada input utilizado para aquele teste.

### Executando testes gerados
É possível rodar o binário, utilizando os inputs gerado pelo klee:

Primeiramente, para rodar os testes gerado pelo klee, gere um códgio gcc linkando libkleeRuntest e sua biblioteca disponível no
klee_build/lib/
```gcc -I ~/klee_src/include -L ~/klee_build/lib/ teste.c -lkleeRuntest```

Basta tão executar o seguinte comando para executar o teste:
```KTEST_FILE=klee-last/test000001.ktest ./a.out```

Com o nosso exemplo acima, ao executar o teste1, o binário irá dar o erro, por causa da divisão por zero, se nós modificarmos o
código, solucionando o erro de divisão por 0, o arquivo teste_2.c, podemos então executar novamente o comando acima que o erro não
irá mais aparecer.
