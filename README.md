
# TeamTalk_BlueBling
## 简介
>本项目主要为了学习teamtalk，尽量将每个细化模块独立出来并编写测试用例，方便移植。

## 环境
>Ubuntu 16.04

## 源码结构说明
 - **slog**：日志模块，用于日志输出(控制台 文件等)  
 -  **config_m**：配置模块，用于读取conf配置  
 - **utilPdu**：pdu工具模块，实现一些数据结构(自己数组等)来处理字节型的数据  
 -  **encDec**: 加密解密模块，用于AES加密解密，计算MD5等  
 -  **imPduBase**：IM基本pdu模块，用于IM消息数据(PDU)处理  
 -  **threadpool**: 线程池模块，提供高效的任务处理方案  
 -  **3rdParty**：第三方库，提供log4cxx、protobuf等库  
 -  **tests**: 测试模块，为各个模块编写的测试用例

## log4cxx库编译安装指南
- 3rdParty/package_log4cxx存放了apache-log4cxx-0.10.0.tar.gz源码包 
- 在3rdParty下执行./make_log4cxx.sh即可  
- 最后会将生成的头文件和库文件拷贝到3rdParty/log4cxx供其他模块使用

- **脚本里的一些注意点：**    
   - log4cxx需依赖apr-devel apr-util-devel  
   - CentOS下执行  
      yum -y install apr-devel  
     yum -y install apr-util-devel  
   - ubuntu下执行  
     apt-get -y install liblog4cxx-dev  
- log4cxx源码包里有些小错误需替换log4cxx源码包里inputstreamreader.cpp、socketoutputstream.cpp、console.cpp，否则编译出错。
这些源文件需添加#include <string.h> #include <stdio.h>


## Ubuntu 16.04 automake1.14编译安装指南
- **automake 1.15卸载解决方案**    
apt-get install autoconf  
使用上述指令ubuntu 16.04会默认安装automake 1.15  
可以用下面指令卸载  
apt-get --purge remove autoconf  

- **通过源码包安装指南**   
依次下载  
http://ftp.gnu.org/gnu/m4/m4-1.4.13.tar.gz  
http://ftp.gnu.org/gnu/autoconf/autoconf-2.65.tar.gz  
http://ftp.gnu.org/gnu/automake/automake-1.14.1.tar.gz  
tar -zxvf xxx.tar.gz  
cd xxx  
./configure [--prefix]  
make
make install  

- **查看安装情况**    
which autoconf  
which automake  

## protobuf库编译安装指南
 - 3rdParty/package_protobuf存放了protobuf-2.6.1.tar.gz源码包    
 - 执行3rdParty/make_protobuf.sh即可    
 - 最后会将生成的头文件和库文件拷贝到3rdParty/pb供其他模块使用    
- **注意点**  
protobuf-2.6.1需依赖automake 1.14，解决方案参考上文ubuntu 16.04 automake1.14编译安装指南  

## 编译运行
- 在主目录下直接执行  
cmake .  
make

- 主目录CMakeLists.txt会调用子模块slog和tests/build下的CMakeLists.txt文件
在slog目录下生成libslog.so，在tests/build下生成各个测试用例

- 可以直接在tests/build下执行cmake,但注意用到libslog.so时需要保证slog目录下已生成该文件


## 模块说明
### 1、TeamTalk slog详解
-  slog模块依赖于log4cxx，实际是对log4cxx的进一步封装。    
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122608722)

### 2、TeamTalk configure详解
- 该项目配置解析为ini配置简化版，基本逻辑是读取文本每一行数据判断是否是name=value结构，然后存入map<string name, string value>。  
 - [查看博客](https://blog.csdn.net/aixiaoql/article/details/122665827)

### 3、TeamTalk UtilPdu详解
- 字节是通过网络传输信息（或在硬盘或内存中存储信息）的单位，该模块实现了了字节流数据处理的一些工具类。  
-  [查看博客](https://blog.csdn.net/aixiaoql/article/details/122666385)

### 4、TeamTalk EncDec详解


### 5、TeamTalk 线程池(threadpool)详解
- 使用现有线程库(pthread.h)实现一个工作线程池，工作线程使用std::list实现任务队列，使用条件变量来解决生产者消费者的竞争问题  
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122601509)
