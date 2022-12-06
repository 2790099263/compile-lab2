echo $1
rm rdcheck
flex scan.l
gcc lex.yy.c rdcheck.c -o rdcheck -lfl
./rdcheck < $1