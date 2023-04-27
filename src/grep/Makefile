all: s21_grep s21_grep_test

s21_grep: s21_grep.c
	gcc -g -Wall -Werror -Wextra -std=c11 -o s21_grep s21_grep.c

s21_grep_test: test.c
	gcc -std=c11 -D_GNU_SOURCE -o s21_grep_test test.c
	touch result.txt
	touch valgrind_result.txt
	chmod ugo+x grep_test1/test_func_grep.sh
	chmod ugo+x grep_test1/test_leak_grep.sh

clean:
	rm -rf s21_grep
	rm -rf s21_grep_test
	rm -rf result.txt
	rm -rf x.txt
	rm -rf y.txt
	rm -rf valgrind_result.txt
	