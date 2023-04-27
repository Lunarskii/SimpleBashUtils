#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkErrors() {
  unsigned long long int count = 0;
  char *buffer = NULL;
  FILE *file;
  size_t length = 0;
  file = fopen("result.txt", "r");
  while (getline(&buffer, &length, file) > 0)
    if (strstr(buffer, "differ")) count++;
  printf("\nErrors found: %lld\n", count);
  free(buffer);
  fclose(file);
}

int main() {
  system("cp /dev/null result.txt");
  system("touch x.txt");
  system("touch y.txt");

  char *array[] = {
      "./s21_cat -b a.txt > x.txt",  // FLAG -b
      "cat -b a.txt > y.txt",
      "./s21_cat -e a.txt > x.txt",  // FLAG -e
      "cat -e a.txt > y.txt",
      "./s21_cat -n a.txt > x.txt",  // FLAG -n
      "cat -n a.txt > y.txt",
      "./s21_cat -s a.txt > x.txt",  // FLAG -s
      "cat -s a.txt > y.txt",
      "./s21_cat -t a.txt > x.txt",  // FLAG -t
      "cat -t a.txt > y.txt",
      "./s21_cat -be a.txt > x.txt",  // FLAG -be
      "cat -be a.txt > y.txt",
      "./s21_cat -bn a.txt > x.txt",  // FLAG -bn
      "cat -bn a.txt > y.txt",
      "./s21_cat -bs a.txt > x.txt",  // FLAG -bs
      "cat -bs a.txt > y.txt",
      "./s21_cat -bt a.txt > x.txt",  // FLAG -bt
      "cat -bt a.txt > y.txt",
      "./s21_cat -en a.txt > x.txt",  // FLAG -en
      "cat -en a.txt > y.txt",
      "./s21_cat -es a.txt > x.txt",  // FLAG -es
      "cat -es a.txt > y.txt",
      "./s21_cat -et a.txt > x.txt",  // FLAG -et
      "cat -et a.txt > y.txt",
      "./s21_cat -ns a.txt > x.txt",  // FLAG -ns
      "cat -ns a.txt > y.txt",
      "./s21_cat -nt a.txt > x.txt",  // FLAG -nt
      "cat -nt a.txt > y.txt",
      "./s21_cat -st a.txt > x.txt",  // FLAG -st
      "cat -st a.txt > y.txt",       "./s21_cat -t test.txt > x.txt",
      "cat -t test.txt > y.txt",     "./s21_cat -e test.txt > x.txt",
      "cat -e test.txt > y.txt",
  };

  char *tests[] = {
      "echo FLAG -b: >> result.txt",
      "echo FLAG -e: >> result.txt",
      "echo FLAG -n: >> result.txt",
      "echo FLAG -s: >> result.txt",
      "echo FLAG -t: >> result.txt",
      "echo FLAG -be >> result.txt",
      "echo FLAG -bn >> result.txt",
      "echo FLAG -bs >> result.txt",
      "echo FLAG -bt >> result.txt",
      "echo FLAG -en >> result.txt",
      "echo FLAG -es >> result.txt",
      "echo FLAG -et >> result.txt",
      "echo FLAG -ns >> result.txt",
      "echo FLAG -nt >> result.txt",
      "echo FLAG -st >> result.txt",
      "echo FLAG nonprintWithTabs >> result.txt",
      "echo FLAG nonprintWithEndl >> result.txt",
  };

  for (int i = 1, counter = 0; i < 36; i += 2, counter++) {
    system(array[i - 1]);
    system(array[i]);

    system(tests[counter]);
    system("diff -qbB x.txt y.txt >> result.txt");
  }

  checkErrors();

  system("rm -rf x.txt");
  system("rm -rf y.txt");
}