out=${1#*es/}
echo "[+]Clang Compile file: ./out_cases/${out%.c*}.txt"
`clang  -fsyntax-only -Xclang -ast-dump $1 | \
sed -r "s/\x1B\[([0-9]{1,3}((;[0-9]{1,3})*)?)?[m|K]//g"  \
> ./out_cases/${out%.c*}.txt`