## Intro
Trabalho para a matéria de automação de teste do CIN/UFPE, será um breve tutorial demonstrando uma simples demostração da
utilização da ferramenta KLEE.

## KLEE
Website: https://klee-se.org/
Descrição: KLEE is a dynamic symbolic execution engine built on top of the LLVM compiler infrastructure, and available under the UIUC open source license

## Dependências
Para a utilização do klee, primeiramente deve ter um compilador para LLVM bitcode, o que pode ser utilizado a ferramente clang
https://clang.llvm.org/ 

## Docker
O klee já disponibilizou uma imagem do docker com todas os intens necessários já instalado e configurado:
link tutorial usando docker: https://klee-se.org/docker/

### tutorial usando o arquivo teste.c desse git de exemplo
É seguindo esse conteiner do docker que demonstrarei o passo a passo. Execute o comando seguinte:/
```docker run --rm -ti --ulimit='stack=-1:-1' klee/klee:3.0```
Fazendo a cópia do arquivo que está nesse repositório;
1º Criar pasta dentro do conteiner: ```mkdir /home/klee/cin``` e ```cd /home/klee/cin```
2º Na sua máquina, pode copiar o arquivo do repositório com o comando ```docker cp teste.c container_id:/home/klee/cin/teste.c```


## Passo a Passo
### Marcar imput como simbólico 
A função o qual está interessado a criar teste deve marcar os inputs simbólicos. Por exemplo, gerar testes para a função
get_sign(int x) tem a seguinte marcação dos inputs simbólicos;
```
int main() {
  int a;
  klee_make_symbolic(&a, sizeof(a), "a");
  return get_sign(a);
}
```

onde que o a, o klee gerará testes com várias variáveis de "a" para percorrer todas as branchs da função get_sign.\
No caso do nosso arquivo do repositório, já está tudo pronto!.

### Compilando o código para LLVM bitcode
usando clang, gerará o get_sign.bc, que é o código em formato de bitcode, o qual o klee interpreta para executar a suas
ferramentas.

Podendo executar o seguinte comando para que o klee consiga coletar mais informações, no nosso caso de exemplo no container,
executar:
```$ clang -I ~/klee_src/include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone teste.c```

ou o seguinte, para situações mais simples:
```$ clang -emit-llvm -c teste.c```

### Rodando Klee
Já com o programa klee instalado na máquina e reconhecido no terminal de comando (variável de path), executar ```$ klee --libc=uclibc --posix-runtime teste.bc```

com isso o klee gerará os testes para a pasta "klee-last"

### Visualizar Klee teste

Para vizualisar os arquivos testes escrito em .ktest, utilizar o comando ```ktest-tool klee-last/test000001.ktest```

### Executando testes gerandos

Para rodar os testes gerado pelo klee, gere um códgio cgg linkando libkleeRuntest
```gcc -I ~/klee_src/include -L ~/klee_build/lib/ teste.c -lkleeRuntest```

Basta tão executar o seguinte comando para executar o teste:
```KTEST_FILE=klee-last/test000001.ktest ./a.out```

