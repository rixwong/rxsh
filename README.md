# rxsh
rxsh是一个教学用，只实现部分基本功能（如I/O重定向，管道）的玩具shell

## 实现功能

- 基本的shell环境
- 支持带有 任意数量参数 的外部命令
- 内建三条内部命令（cd，help，exit）
- 支持标准IO重定向（>, >>, < 三种)
- 支持多级连接的管道（例：cat < buildin_cmd.c | grep printf | wc -l)

## shell执行逻辑大致如下

```c
	while(1) {
		prompt();
		read_line();
		parse_line();
		run();
	}
```

对于指令执行run()逻辑如下

```
	splitByPipeSymbol
	fork
		child：
			deal_pipe1
			run_without_pipe()
		father:
			deal_pipe2
```

对于run_without_pipe()逻辑如下

```
	checkIfRedirection()
	redirect()
	exec()
```

用到的系统调用及相关函数：


- fork()
- 建立管道 pipe()
- 复制文件描述符 dup() dup2()
- 打开关闭文件 open() close()
- 切换工作目录 chdir()

## 语法限定

其他参数限定：

- 重定向符号与文件名之间必须要有空白符分隔， 且重定向符必须排在所有参数最后面 例： 'ls -al > 1.txt' 本shell可正确解析
- 类似 'ls | |' 这种非法输入（管道符号后无指令）， 能够发现并给出提示信息
- 不支持引号(')(")
- 支持 重定向符和管道符号（语法合法）同时出现， 例：cat < buildin_cmd.c | grep printf | wc -l
Ctrl-D的行为保持与bash相同，即退出shell

## 如何编译/构建

**本地编译环境： gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0**

若其他环境重新编译失败，请upgrade gcc版本，保证getline函数可使用。

已经写好Makefile，故

```shell
	make clean
	make
```

即可重新编译
