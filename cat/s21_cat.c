#include "s21_cat.h"

int main(int argc, char *argv[]) {
  options flags = CatParseArgs(argc, argv);

  argv += optind;
  argc -= optind;

  if (!argc) {
    fprintf(stderr, "No File\n");
  } else {
    const char *table[256];

    CatSetTable(table);
    if (flags.endlStrings) CatSetEndlStrings(table);
    if (flags.showTabs) CatSetTabs(table);
    if (flags.showNonPrintableSymbols) CatSetNonPrintable(table);

    CatWithArgs(argc, argv, flags, table);
  }

  return 0;
}

void CatSetTable(const char *table[256]) {
  const char *rawTable[] = {
      "\00",  "\01",  "\02",  "\03",  "\04",  "\05",  "\06",  "\07",  "\b",
      "\t",   "\n",   "\v",   "\014", "\r",   "\016", "\017", "\020", "\021",
      "\022", "\023", "\024", "\025", "\026", "\027", "\030", "\031", "\032",
      "\033", "\034", "\035", "\036", "\037", " ",    "!",    "\"",   "#",
      "$",    "%",    "&",    "\'",   "(",    ")",    "*",    "+",    ",",
      "-",    ".",    "/",    "0",    "1",    "2",    "3",    "4",    "5",
      "6",    "7",    "8",    "9",    ":",    ";",    "<",    "=",    ">",
      "?",    "@",    "A",    "B",    "C",    "D",    "E",    "F",    "G",
      "H",    "I",    "J",    "K",    "L",    "M",    "N",    "O",    "P",
      "Q",    "R",    "S",    "T",    "U",    "V",    "W",    "X",    "Y",
      "Z",    "[",    "\\",   "]",    "^",    "_",    "`",    "a",    "b",
      "c",    "d",    "e",    "f",    "g",    "h",    "i",    "j",    "k",
      "l",    "m",    "n",    "o",    "p",    "q",    "r",    "s",    "t",
      "u",    "v",    "w",    "x",    "y",    "z",    "{",    "|",    "}",
      "~",    "\x7f", "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86",
      "\x87", "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e", "\x8f",
      "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", "\x98",
      "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e", "\x9f", "\xa0", "\xa1",
      "\xa2", "\xa3", "\xa4", "\xa5", "\xa6", "\xa7", "\xa8", "\xa9", "\xaa",
      "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0", "\xb1", "\xb2", "\xb3",
      "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9", "\xba", "\xbb", "\xbc",
      "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5",
      "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce",
      "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
      "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf", "\xe0",
      "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7", "\xe8", "\xe9",
      "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef", "\xf0", "\xf1", "\xf2",
      "\xf3", "\xf4", "\xf5", "\xf6", "\xf7", "\xf8", "\xf9", "\xfa", "\xfb",
      "\xfc", "\xfd", "\xfe", "\xff"};

  memcpy(table, rawTable, sizeof(rawTable));
}

void CatSetEndlStrings(const char *table[256]) { table['\n'] = "$\n"; }

void CatSetTabs(const char *table[256]) { table['\t'] = "^I"; }

void CatSetNonPrintable(const char *table[256]) {
  const char *symbols[] = {"^@", "^A", "^B", "^C", "^D",
                           "^E", "^F", "^G", "^H"};
  const char *symbols2[] = {"^K", "^L", "^M", "^N",  "^O", "^P", "^Q",
                            "^R", "^S", "^T", "^U",  "^V", "^W", "^X",
                            "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
  const char *symbols3[] = {
      "^?",   "M-^@", "M-^A",  "M-^B", "M-^C", "M-^D", "M-^E", "M-^F", "M-^G",
      "M-^H", "M-^I", "M-^J",  "M-^K", "M-^L", "M-^M", "M-^N", "M-^O", "M-^P",
      "M-^Q", "M-^R", "M-^S",  "M-^T", "M-^U", "M-^V", "M-^W", "M-^X", "M-^Y",
      "M-^Z", "M-^[", "M-^\\", "M-^]", "M-^^", "M-^_", "M- ",  "M-!",  "M-\"",
      "M-#",  "M-$",  "M-%",   "M-&",  "M-'",  "M-(",  "M-)",  "M-*",  "M-+",
      "M-,",  "M--",  "M-.",   "M-/",  "M-0",  "M-1",  "M-2",  "M-3",  "M-4",
      "M-5",  "M-6",  "M-7",   "M-8",  "M-9",  "M-:",  "M-;",  "M-<",  "M-=",
      "M->",  "M-?",  "M-@",   "M-A",  "M-B",  "M-C",  "M-D",  "M-E",  "M-F",
      "M-G",  "M-H",  "M-I",   "M-J",  "M-K",  "M-L",  "M-M",  "M-N",  "M-O",
      "M-P",  "M-Q",  "M-R",   "M-S",  "M-T",  "M-U",  "M-V",  "M-W",  "M-X",
      "M-Y",  "M-Z",  "M-[",   "M-\\", "M-]",  "M-^",  "M-_",  "M-`",  "M-a",
      "M-b",  "M-c",  "M-d",   "M-e",  "M-f",  "M-g",  "M-h",  "M-i",  "M-j",
      "M-k",  "M-l",  "M-m",   "M-n",  "M-o",  "M-p",  "M-q",  "M-r",  "M-s",
      "M-t",  "M-u",  "M-v",   "M-w",  "M-x",  "M-y",  "M-z",  "M-{",  "M-|",
      "M-}",  "M-~",  "M-^?"};

  memcpy(table, symbols, sizeof(symbols));
  memcpy(&table[11], symbols2, sizeof(symbols2));
  memcpy(&table[127], symbols3, sizeof(symbols3));
}

options CatParseArgs(int argc, char *argv[]) {
  options flags = {false, false, false, false, false, false};
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};
  int currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);

  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) {
    switch (currentFlag) {
      case 'b':
        flags.numberingNoEmptyStrings = true;
        break;
      case 'e':
        flags.endlStrings = true;
        flags.showNonPrintableSymbols = true;
        break;
      case 'v':
        flags.showNonPrintableSymbols = true;
        break;
      case 'E':
        flags.endlStrings = true;
        break;
      case 'n':
        flags.numberingAllStrings = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
      case 'T':
        flags.showTabs = true;
        flags.showNonPrintableSymbols = true;
        break;
    }
  }

  return flags;
}

void CatWorkingWithFlags(FILE *file, options flags, counters *count,
                         const char *table[256]) {
  int c = 0;
  char lastSymbol = '\n';
  bool squeeze = false;

  while (fread(&c, 1, 1, file) > 0) {
    if (lastSymbol == '\n') {
      if (flags.squeeze && c == '\n') {
        if (squeeze) {
          continue;
        } else {
          squeeze = true;
        }
      } else {
        squeeze = false;
      }
      if (flags.numberingNoEmptyStrings) {
        if (c != '\n') {
          printf("%6lld\t", ++(count->countStrings));
        }
      } else if (flags.numberingAllStrings) {
        printf("%6lld\t", ++(count->countStrings));
      }
    }
    if (!(*table[c])) {
      printf("%c", '\0');
    } else {
      printf("%s", table[c]);
    }
    lastSymbol = c;
  }
}

void CatWithArgs(int argc, char **argv, options flags, const char *table[256]) {
  counters count = {0};

  for (char **fileName = argv; argc; fileName++, argc--) {
    FILE *file;

    if (**fileName == '-') continue;
    if ((file = fopen(*fileName, "rb")) == NULL) {
      fprintf(stderr, "./s21_cat: %s: No such file or directory\n", *fileName);
      continue;
    }
    CatWorkingWithFlags(file, flags, &count, table);
    if (argc - 1 != 0) printf("\n");
    fclose(file);
  }
}
