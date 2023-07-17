all: s21_cat s21_cat_test

s21_cat: s21_cat.c
	gcc -g -Wall -Werror -Wextra -std=c11 -o s21_cat s21_cat.c

s21_cat_test: test.c
	gcc -std=c11 -D_GNU_SOURCE -o s21_cat_test test.c

clean:
	rm -rf s21_cat
	rm -rf s21_cat_test
	rm -rf result.txt
	rm -rf x.txt
	rm -rf y.txt