echo $1
rm rdcheck
flex scan.l
gcc lex.yy.c rdparser.c -o rdparser -lfl
./rdparser < $1