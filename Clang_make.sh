echo "即将输出文件$1的clang编译ast:"
out=${1#*es/}
`clang  -fsyntax-only -Xclang -ast-dump $1 | sed -r "s/\x1B\[([0-9]{1,3}((;[0-9]{1,3})*)?)?[m|K]//g" > ./out_cases/${out%.c*}.txt`