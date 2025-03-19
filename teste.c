#include <klee/klee.h>

int calcula_valor(int a, int b, int c, int d) {
  int resultado = 0;
  int aux1 = a + b - c;
  int aux2 = d * 2 + a / (b + 1);
  int aux3 = (c % 3) * (d & 7);

  if (b > 10) {
    resultado = (a * (b + 1)) + (b % 3) - 5 + aux1;
  } else if (b < -10) {
    resultado = (a - (b - 1)) * 2 + (b & 7) + aux2;
  }

  resultado -= aux3;

  if (resultado > 50) {
    resultado -= (a % 7) + aux1;
  } else if (resultado < -50) {
    resultado += (b % 5) - aux2;
  }

  resultado += (aux1 * aux2) / (aux3 + 1);

  return resultado;
}

int main() {
  int a, b, c, d;
  klee_make_symbolic(&a, sizeof(a), "a");
  klee_make_symbolic(&b, sizeof(b), "b");
  klee_make_symbolic(&c, sizeof(c), "c");
  klee_make_symbolic(&d, sizeof(d), "d");

  int resultado = calcula_valor(a, b, c, d);

  return 0;
}
