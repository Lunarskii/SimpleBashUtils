    #include "s21_grep.h"

int main(int argc, char *argv[]) {
    options flags = GrepParseArgs(argc, argv);

    argv += optind;
    argc -= optind;
    if (!argc) {
        fprintf(stderr, "No File or Pattern\n");
        if (flags.pattern != NULL) free(flags.pattern);
    } else {
        GrepWithArgs(argc, argv, flags);
    }

    return 0;
}

void deleteEndLine(char *buffer) {
    for (; *buffer != '\0'; buffer++) {
        printf("%c", *buffer);
    }
    printf("\n");
}

void appendPattern(options *flags, char *expression) {
    size_t patternSize = strlen(flags->pattern);
    size_t expressionSize = strlen(expression);
    int temp = 1;

    if (patternSize) temp = 2;

    flags->pattern = realloc(flags->pattern, (patternSize + expressionSize + temp) * sizeof(char));
    if (temp == 2) strcat(flags->pattern, "|");
    strcat(flags->pattern, expression);
}

options GrepParseArgs(int argc, char **argv) {
    options flags = {REG_EXTENDED, NULL, 0, 0, 0, 0, 0, 0, 0, 0};
    int currentArg = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0);

    flags.pattern = malloc(sizeof(char));
    flags.pattern[0] = '\0';

    for (; currentArg != -1;
        currentArg = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) {
        switch (currentArg) {
        case 'e':
            flags.patternAvailable = 1;
            appendPattern(&flags, optarg);
            break;
        case 'i':
            flags.REG_FLAGS |= REG_ICASE;
            break;
        case 'v':
            flags.invert = true;
            break;
        case 'c':
            flags.outputQuantityMatchingStrings = true;
            break;
        case 'l':
            flags.outputMatchingFiles = true;
            break;
        case 'n':
            flags.outputNumbersFoundRows = true;
            break;
        case 'h':
            flags.outputMatchingStringsWithoutFilename = true;
            break;
        case 's':
            flags.deleteErrorMessage = true;
            break;
        case 'f':
            flags.patternAvailable = -1;
            FILE *file;
            size_t length;
            char *buffer = NULL;

            if ((file = fopen(optarg, "rb")) == NULL) {
                fprintf(stderr, "./s21_grep: %s: No such file or directory\n", optarg);
            } else {
                while (getline(&buffer, &length, file) > 0) {
                    int size = strlen(buffer);
                    if (buffer[size - 1] == '\n') buffer[size - 1] = '\0';
                    appendPattern(&flags, buffer);
                }
                if (buffer != NULL) free(buffer);
                fclose(file);
            }
            break;
        case 'o':
            flags.printMatchingNoEmptyStrings = true;
            break;
        }
    }
    int patternSize = strlen(flags.pattern);
    if (patternSize) {
        flags.patternAvailable = patternSize;
    }

    return flags;
}

int print(options *flags, counters *count, char *fileName, unsigned long long int countStrings, char *buffer) {
    if (printFileName(flags, count, fileName) == 0) {
        printf("\n");
        return 0;
    }
    if (!(flags->printMatchingNoEmptyStrings)) {
        printStringNumber(flags, countStrings);
        printString(flags, buffer);
    }

    return 1;
}

int printFileName(options *flags, counters *count, char *fileName) {
    if ((flags->outputMatchingFiles || count->quantityFiles)) {
        if (flags->outputMatchingFiles) {
            printf("%s", fileName);
            return 0;
        } else if (!(flags->outputQuantityMatchingStrings) && !(flags->outputMatchingStringsWithoutFilename)) {
            if (!(flags->printMatchingNoEmptyStrings) || count->oEnabled) {
                printf("%s:", fileName);
                return 1;
            }
        }
    }
    return 1;
}

void printStringNumber(options *flags, unsigned long long int countStrings) {
    if (flags->outputNumbersFoundRows && !(flags->outputMatchingFiles) && !(flags->outputQuantityMatchingStrings)) {
        printf("%lld:", countStrings);
    }
}

void printString(options *flags, char *buffer) {
    if (!(flags->outputQuantityMatchingStrings)) {
        deleteEndLine(buffer);
    }
}

int o_option(options *flags, counters *count, char *fileName, int countStrings, char *buffer, regex_t *regex, regmatch_t *pmatch) {
    count->oEnabled = true;
    if (printFileName(flags, count, fileName) == 0) {
        printf("\n");
        return 0;
    }
    printStringNumber(flags, countStrings);
    printf("%.*s", (int)(pmatch->rm_eo - pmatch->rm_so), buffer + pmatch->rm_so);
    char *left = buffer + pmatch->rm_eo;
    printf("\n");

    while (!regexec(regex, left, 1, pmatch, 0)) {
        if (printFileName(flags, count, fileName) == 0) {
            printf("\n");
            return 0;
        }
        printStringNumber(flags, countStrings);
        printf("%.*s", (int)(pmatch->rm_eo - pmatch->rm_so), left + pmatch->rm_so);
        left = left + pmatch->rm_eo;
        printf("\n");
    }

    return 1;
}

int GrepWorkingWithArgs(options *flags, counters *count, char *fileName, FILE *file, regex_t *regex) {
    unsigned long long int countStrings = 0;
    unsigned long long int countMatchingStrings = 0;
    size_t length = 0;
    char *buffer = NULL;
    regmatch_t pmatch;

    while (getline(&buffer, &length, file) > 0) {
        if (flags->outputNumbersFoundRows) countStrings++;

        int size = strlen(buffer);
        if (buffer[size - 1] == '\n') buffer[size - 1] = '\0';

        if (flags->invert && regexec(regex, buffer, 1, &pmatch, 0)) {
        if (flags->outputQuantityMatchingStrings) countMatchingStrings++;

        if (print(flags, count, fileName, countStrings, buffer) == 0) {
            if (buffer != NULL) free(buffer);
            return 0;
        }
        } else if (!flags->invert && !regexec(regex, buffer, 1, &pmatch, 0)) {
        if (flags->outputQuantityMatchingStrings) countMatchingStrings++;

        if (flags->printMatchingNoEmptyStrings && !flags->outputQuantityMatchingStrings) {
            if (o_option(flags, count, fileName, countStrings, buffer, regex, &pmatch) == 0) {
                if (buffer != NULL) free(buffer);
                return 0;
            }
        } else {
            if (print(flags, count, fileName, countStrings, buffer) == 0) {
                if (buffer != NULL) free(buffer);
                return 0;
            }
        }
        }
    }

    if (flags->outputQuantityMatchingStrings && !(flags->outputMatchingFiles)) {
        if (count->quantityFiles > 0 && !(flags->outputMatchingStringsWithoutFilename)) {
            printf("%s:", fileName);
        }
        printf("%lld\n", countMatchingStrings);
    }

    if (buffer != NULL) free(buffer);
    return 0;
}

int GrepWithArgs(int argc, char *argv[], options flags) {
    counters count = {0, 0};
    regex_t regex;
    char **end = &argv[argc];

    if (flags.patternAvailable == 0) {
        count.quantityFiles = argc - 1;
        if (regcomp(&regex, argv[0], flags.REG_FLAGS)) {
        fprintf(stderr, "Failed to compile regex\n");
        if (flags.pattern != NULL) free(flags.pattern);
        return 1;
        }
    } else if (flags.patternAvailable > 0) {
        count.quantityFiles = argc;
        if (regcomp(&regex, flags.pattern, flags.REG_FLAGS)) {
        fprintf(stderr, "Failed to compile regex\n");
        if (flags.pattern != NULL) free(flags.pattern);
        return 1;
        }
    } else {
        if (flags.pattern != NULL) free(flags.pattern);
        return 1;
    }
    if (flags.pattern != NULL) free(flags.pattern);

    if (count.quantityFiles == 1) count.quantityFiles = 0;

    for (char **fileName = argv + (flags.patternAvailable ? 0 : 1); fileName != end; fileName++) {
        FILE *file;

        if ((file = fopen(*fileName, "rb")) == NULL) {
        if (!(flags.deleteErrorMessage))
            fprintf(stderr, "./s21_grep: %s: No such file or directory\n", *fileName);
        (count.quantityFiles)--;
        continue;
        }

        GrepWorkingWithArgs(&flags, &count, *fileName, file, &regex);

        fclose(file);
        (count.quantityFiles)--;
    }

    regfree(&regex);
    return 0;
}
