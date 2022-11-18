# 编译原理实验二

## 任务内容

1. 学习所提供的“表达式文法”的递归下降处理理解 lex.l、rdparser.c 的内容在 vscode/Clion 中建立工程并调试运行
2. 学习所提供的文法与词法分析所提供的文法作比较
3. 编写 rdgram 所提供文法的递归下降程序
   1. 编写不生成“语法树”的递归下降程序 rdcheck.c
   2. 将 rdcheck.c 改造为生成语法树的递归下降程序 rdparser.c
   3. 改进 词法分析程序、showAst 函数、main 函数等，使递归下降程序 rdparser 最终从命令行读取要分析的程序 test.c,分析后调用 showAst 打印该程序的结构。