#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void appendSymbol(char *string, char symbol) {
  char *buffer = malloc(sizeof(char) * 2);
  buffer[0] = symbol;
  buffer[1] = '\0';
  strcat(string, buffer);

  free(buffer);
}

void checkErrors() {
  unsigned long long int count = 0;
  char *buffer = NULL;
  FILE *file;
  size_t length = 0;
  file = fopen("result.txt", "r");
  while (getline(&buffer, &length, file) > 0)
    if (strstr(buffer, "differ")) count++;
  printf("\nErrors found: %lld", count);
  fclose(file);

  file = fopen("valgrind_result.txt", "r");
  while (getline(&buffer, &length, file) > 0)
    if (strstr(buffer, "Files x.txt and y.txt differ")) count++;
  printf("\nValgrind errors found: %lld\n", count);

  free(buffer);
}

void f_test() {
  system("echo MANUAL F TEST STARTED");

  system("touch empty.txt");
  system("./s21_grep -f empty.txt a.txt > x.txt");
  system("grep -f empty.txt a.txt > y.txt");
  system("echo TEST F1: >> result.txt");
  system("diff -qbB x.txt y.txt >> result.txt");

  system("./s21_grep -f enter_patterns.txt a.txt > x.txt");
  system("grep -f enter_patterns.txt a.txt > y.txt");
  system("echo TEST F2: >> result.txt");
  system("diff -qbB x.txt y.txt >> result.txt");

  system("./s21_grep -f enter_patterns.txt a.txt b.txt > x.txt");
  system("grep -f enter_patterns.txt a.txt b.txt > y.txt");
  system("echo TEST F3: >> result.txt");
  system("diff -qbB x.txt y.txt >> result.txt");

  system("./s21_grep -f enter_patterns.txt a.txt b.txt c.txt > x.txt");
  system("grep -f enter_patterns.txt a.txt b.txt c.txt > y.txt");
  system("echo TEST F4: >> result.txt");
  system("diff -qbB x.txt y.txt >> result.txt");

  system("rm -rf empty.txt");
  checkErrors();
}

void appendString(char *string, int count) {
  char string2[100];
  snprintf(string2, 100, "%d", count);
  strcat(string, string2);
}

int main(int argc, char *argv[]) {
  system("cp /dev/null result.txt");
  system("cp /dev/null valgrind_result.txt");

  const char alphabet[] = "eivclnho";
  const size_t last_digit = sizeof(alphabet) - 2;
  const size_t length = 2;
  size_t number[length];

  system("echo RANDOM TEST STARTED");

  for (size_t i = 0; i < length; ++i) number[i] = 0;
  while (1) {
    char *systemString = malloc(sizeof(char) * 100);
    char *systemString2 = malloc(sizeof(char) * 100);
    char *systemString3 = malloc(sizeof(char) * 100);
    char *systemString4 = malloc(sizeof(char) * 100);
    char *systemString5 = malloc(sizeof(char) * 150);
    char string[8];
    for (size_t i = 0; i < length; ++i)
      appendSymbol(string,
                   alphabet[number[i]]);  // putchar(alphabet[number[i]]);

    strcat(systemString, "./s21_grep -s");
    strcat(systemString, string);
    strcat(systemString, " txt$");
    strcat(systemString, " a.txt");
    strcat(systemString5,
           "valgrind --track-origins=yes --leak-check=full "
           "--show-leak-kinds=all --show-reachable=yes -s ");
    strcat(systemString5, systemString);
    strcat(systemString, " > x.txt");

    strcat(systemString2, "grep -s");
    strcat(systemString2, string);
    strcat(systemString2, " txt$");
    strcat(systemString2, " a.txt");
    strcat(systemString2, " > y.txt");

    system(systemString);
    system(systemString2);
    printf("%s\n", systemString);
    printf("%s\n", systemString2);

    strcat(systemString3, "printf \"TEST -");
    strcat(systemString3, string);
    strcat(systemString4, systemString3);
    strcat(systemString3, ": \" >> result.txt");
    strcat(systemString4, ": \" >> valgrind_result.txt");
    system(systemString3);
    system(systemString4);
    system("echo >> valgrind_result.txt");
    system("echo >> valgrind_result.txt");
    system("echo >> valgrind_result.txt");
    system("echo >> result.txt");
    system("diff -q x.txt y.txt >> result.txt");
    strcat(systemString5, " >> valgrind_result.txt 2>&1");
    system(systemString5);
    system("echo >> valgrind_result.txt");
    system("echo >> valgrind_result.txt");

    size_t pos = length;
    while (number[--pos] == last_digit) {
      if (pos == 0) {
        free(systemString);
        free(systemString2);
        free(systemString3);
        free(systemString4);
        free(systemString5);
        system("rm -rf x.txt");
        system("rm -rf y.txt");
        f_test();
        return 0;
      }
      number[pos] = 0;
    }
    number[pos]++;

    for (size_t i = 0; i < length; i++) string[i] = 0;
  }
}