echo $1
rm rdparser
flex scan.l
gcc lex.yy.c rdparser.c -o rdparser -lfl
./rdparser < $1