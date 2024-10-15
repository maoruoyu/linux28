安装好的so动态库文件在/usr/local/lib目录下         
安装需要提前准备好编译环境、工具（make 、cmake 、gcc 、g++）
libevent包解压在~/libevent-2.1.12-stable，
步骤：
    ./configure		检查安装环境 生成 makefile           注意运行显示缺少openssl库要去下载最新的openssl，   sudo apt-get update                                   sudo apt-get install libssl-dev

	make			生成 .o 和 可执行文件

	sudo make install	将必要的资源cp置系统指定目录。
