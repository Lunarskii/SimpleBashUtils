#ifndef _SRC_CAT_S21_CAT_H
#define _SRC_CAT_S21_CAT_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  bool numberingNoEmptyStrings;
  bool endlStrings;
  bool numberingAllStrings;
  bool squeeze;
  bool showTabs;
  bool showNonPrintableSymbols;
} options;

typedef struct {
  unsigned long long int countStrings;
} counters;

void CatSetTable(const char *table[256]);
void CatSetEndlStrings(const char *table[256]);
void CatSetTabs(const char *table[256]);
void CatSetNonPrintable(const char *table[256]);
options CatParseArgs(int argc, char *argv[]);
void CatWorkingWithFlags(FILE *file, options flags, counters *count,
                         const char *table[256]);
void CatWithArgs(int argc, char **argv, options flags, const char *table[256]);

#endif  // _SRC_CAT_S21_CAT_H
