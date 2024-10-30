安装好的so动态库文件在/usr/local/lib目录下         
安装需要提前准备好编译环境、工具（make 、cmake 、gcc 、g++）
libevent包解压在~/libevent-2.1.12-stable，
步骤：
    ./configure		检查安装环境 生成 makefile           注意运行显示缺少openssl库要去下载最新的openssl，   sudo apt-get update                                   sudo apt-get install libssl-dev

	make			生成 .o 和 可执行文件

	sudo make install	将必要的资源cp置系统指定目录。


event代码分析：

在分析这两组代码的行为时，我们需要考虑管道的特性、阻塞与非阻塞模式的区别，以及事件驱动模型的影响。让我们详细分析一下为什么 myread 和 mywrite 这组代码与 read_fifo 和 write_fifo 这组代码的行为不同。
代码分析
myread 和 mywrite
非阻塞模式：
myread 使用 O_RDONLY | O_NONBLOCK 打开管道进行读取。
mywrite 使用 O_WRONLY | O_NONBLOCK 打开管道进行写入。
事件驱动：
myread 使用 EV_READ | EV_PERSIST，意味着即使没有新数据，事件仍然会被 libevent 认为是可读的。
mywrite 使用 EV_WRITE，但没有 EV_PERSIST，这意味着写事件只会触发一次。

行为分析：
在非阻塞模式下，read 在没有数据可读时会立即返回 -1，并设置 errno 为 EAGAIN。
如果 read 没有正确处理返回值，缓冲区 buf 的内容可能没有被清空，导致旧数据被重复打印。
read_fifo 和 write_fifo
阻塞模式：
read_fifo 使用 O_RDONLY 打开管道进行读取。
write_fifo 使用 O_WRONLY 打开管道进行写入。
事件驱动：
read_fifo 使用 EV_READ | EV_PERSIST，事件会持续触发。
write_fifo 使用 EV_WRITE | EV_PERSIST，事件会持续触发。
行为分析：
在阻塞模式下，read 会在没有数据可读时阻塞，直到有数据可读或写端关闭。
当写端关闭且没有数据可读时，read 会返回 0，表示 EOF。
为什么行为不同
非阻塞模式的影响：
在 myread 中，非阻塞模式导致 read 在没有数据可读时返回 -1，但由于没有正确处理返回值，缓冲区可能保留旧数据。
这导致 myread 在写端关闭后仍然打印旧数据。
阻塞模式的影响：
在 read_fifo 中，阻塞模式确保 read 只有在有数据可读时才返回。
当写端关闭且没有数据可读时，read 返回 0，缓冲区被清空。
事件处理的差异：
mywrite 没有使用 EV_PERSIST，可能导致写事件只触发一次。
write_fifo 使用 EV_PERSIST，确保写事件持续触发。


