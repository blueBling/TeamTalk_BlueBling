
# TeamTalk_BlueBling
## 简介
>本项目主要为了学习teamtalk，尽量将每个细化模块独立出来并编写测试用例，方便移植。

## 环境
>Ubuntu 16.04

## 源码结构说明
 -  **base/SLog**：日志模块，用于日志输出(控制台 文件等)  
 -  **base/Config**：配置模块，用于读取conf配置  
 -  **base/UtilPdu**：pdu工具模块，实现一些数据结构(自己数组等)来处理字节型的数据  
 -  **base/EncDec**: 加密解密模块，用于AES加密解密，计算MD5等  
 -  **base/ImPduBase**：IM基本pdu模块，用于IM消息数据(PDU)处理  
 -  **base/ThreadPool**: 线程池模块，提供高效的任务处理方案  
 -  **base/Lock**: 锁模块，使用pthread.h封装了普通锁、读写锁、普通局部锁和读写局部锁  
 -  **base/NetLib**: 网络库，主要用于处理tcp连接  
 -  **base/HttpParser**: HTTP解析库，利用了ngnix的http_parse解析库  
 -  **base/ImConn**: tcp连接基础类库，利用netlib封装了一套比较通用的tcp连接基础类，业务类的连接可继承该类重写自己的业务逻辑
 -  **base/HttpClient**: HTTP客户端基础类库，利用curl库和jsoncpp实现了http客户端Get/Post和语音数据的上传下载
 -  **3rdParty**：第三方库，提供log4cxx、protobuf等库  
 -  **tests_xxx**: 测试模块，为各个模块编写的测试用例

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

- 注意用到libslog.so时需要保证slog目录下已生成该文件

## 模块说明
### 1、TeamTalk SLog详解
-  SLog模块依赖于log4cxx，实际是对log4cxx的进一步封装。    
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122608722)

### 2、TeamTalk Config详解
- 该项目配置解析为ini配置简化版，基本逻辑是读取文本每一行数据判断是否是name=value结构，然后存入map<string name, string value>。  
 - [查看博客](https://blog.csdn.net/aixiaoql/article/details/122665827)

### 3、TeamTalk UtilPdu详解
- 字节是通过网络传输信息（或在硬盘或内存中存储信息）的单位，该模块实现了了字节流数据处理的一些工具类。  
-  [查看博客](https://blog.csdn.net/aixiaoql/article/details/122666385)

### 4、TeamTalk IMPduBase详解
- 用于TeamTalk通讯数据包的读取解析 依赖于UtilPdu工具类，依赖于protobuf第三方库
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122669517)

### 5、TeamTalk EncDec详解
- 主要用于AES加解密和MD5计算
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122670155)

### 6、TeamTalk 线程池(ThreadPool)详解
- 使用现有线程库(pthread.h)实现一个工作线程池，工作线程使用std::list实现任务队列，使用条件变量来解决生产者消费者的竞争问题  
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122601509)

### 7、TeamTalk Lock详解
- 使用pthread.h封装了普通锁、读写锁、普通局部锁和读写局部锁
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122679771)

### 8、NetLib详解
- 主要用于处理tcp连接，自己实现了一个网络库
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122687021)

### 9、HttpParser详解
- TeamTalk http解析实际上是利用了ngnix的http_parse解析库
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122862322)

### 10、IMConn详解
- 利用netlib封装了一套比较通用的tcp连接基础类，业务类的连接像CLoginConn用于登录服务器连接和CHttpConn用于http连接等可继承该类重写自己的业务逻辑
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122936891)

### 11、HttpClient详解
- HttpClient利用curl库和jsoncpp实现了http客户端Get/Post和语音数据的上传下载；http body主要为json格式，jsoncpp主要服务于该json数据的组装和解析
- [查看博客](https://blog.csdn.net/aixiaoql/article/details/122948237)
