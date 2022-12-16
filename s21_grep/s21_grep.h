#ifndef _SRC_GREP_S21_GREP_H
#define _SRC_GREP_S21_GREP_H
#define _GNU_SOURCE  // getline

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int REG_FLAGS;
  char *pattern;
  int patternAvailable;
  bool invert;
  bool outputQuantityMatchingStrings;
  bool outputMatchingFiles;
  bool outputNumbersFoundRows;
  bool outputMatchingStringsWithoutFilename;
  bool deleteErrorMessage;
  bool printMatchingNoEmptyStrings;
} options;

typedef struct {
  unsigned long long int quantityFiles;
  bool oEnabled;
} counters;

void deleteEndLine(char *buffer);
void appendPattern(options *flags, char *expression);
options GrepParseArgs(int argc, char **argv);
int printFileName(options *flags, counters *count, char *fileName);
void printStringNumber(options *flags, unsigned long long int countStrings);
void printString(options *flags, char *buffer);
int o_option(options *flags, counters *count, char *fileName, int countStrings, char *buffer, regex_t *regex, regmatch_t *pmatch);
int GrepWorkingWithArgs(options *flags, counters *count, char *fileName, FILE *file, regex_t *regex);
int GrepWithArgs(int argc, char *argv[], options flags);

#endif  // _SRC_GREP_S21_GREP_H
