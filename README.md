# TeamTalk_coderl

[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

## 简介

本项目主要为了学习teamtalk，尽量将每个细化模块独立出来并编写测试用例，方便移植。


## 项目结构（后续将继续补充）
```
.
├── CMakeLists.txt
├── config_m
│   ├── ConfigFileReader.cpp
│   └── ConfigFileReader.h
├── encDec
│   ├── Base64.cpp
│   ├── Base64.h
│   ├── EncDec.cpp
│   ├── EncDec.h
│   ├── UtilPdu.cpp
│   └── UtilPdu.h
├── slog
│   ├── CMakeLists.txt
│   ├── include
│   │   └── log4cxx头文件 ...
│   ├── lib
│   │   ├── liblog4cxx.so
│   │   ├── liblog4cxx.so.10
│   │   └── liblog4cxx.so.10.0.0
│   ├── log4cxx.properties
│   ├── slog_api.cpp
│   └── slog_api.h
├── tests
│   ├── build
│   │   ├── CMakeLists.txt
│   │   ├── dbproxyserver.conf
│   │   └── log4cxx.properties
│   ├── test_config.cpp
│   ├── test_encdec.cpp
│   ├── test_slog.cpp
│   └── test_utilpdu.cpp
├── tree.md
└── utilPdu
    ├── UtilPdu.cpp
    └── UtilPdu.h
```

## 模块说明
### 一、TeamTalk slog详解

#### 1.简介
> slog模块依赖于log4cxx，实际是对log4cxx的进一步封装。

#### 2.主要类关系说明：
> CSLog：teamtalk日志类，利用内部成员(CSLogObject* m_log;)调用CSLogObject接口(组合)
> CSLogObject: 日志对象虚基类，内部接口全为虚函数(实现为空)，主要由子类去实现
> CLog4CXX: 继承于CSLogObject，利用内部成员(LoggerPtr m_logger;//log4cxx库)通过log4cxx接口实现自己的接口(重写父类接口)
>
> TeamTalk slog，在util定义了CSLog g_imlog;全局变量，实现全局唯一(一般日志类都是单例)
>
> 主要分为两个模块slog和tests,主目录用于生成测试用例，slog模块主要生成so供其他模块使用
> 必须包含log4cxx.properties，否则使用log4cxx时会找不到该配置文件而报错
```
CLog4CXX::CLog4CXX(const char* module_name, int delay) : CSLogObject(module_name, delay)
{
    PropertyConfigurator::configureAndWatch("log4cxx.properties", delay);
    m_logger = Logger::getLogger(module_name);
}
```
> Log4cxx配置器有一个configureAndWatch()方法，该方法启动一个Thread来观察用于配置的文件(XML文件或properties文件)，
> 并在文件更改时触发重新配置，多久检查一次取决于第二个参数delay(milliseconds)。

#### 3.编译运行
```
cd slog
cmake .
make
当前目录生成libslog.so，供其他模块使用
```

### 二、TeamTalk configure详解
 > INI文件的格式很简单，最基本的三个要素是：parameters，sections和comments  
 > parameters:name = value  
 > sections: [section]  
 > comments: ;comments text  

#### 1.简介
> 此配置解析为ini配置简化版，
> 基本逻辑是读取文本每一行数据判断是否是name=value结构，然后存入map<string name, string value>。

#### 2.基本接口说明
```
CConfigFileReader构造函数：加载配置文件并将所有配置解析出来，存入m_config_map
char* GetConfigName(const char* name)：从m_config_map中查找名字为name的配置
int SetConfigValue(const char* name, const char*  value)：修改m_config_map中信息，并将m_config_map内信息重新写入配置文件
```

### 三、TeamTalk UtilPdu详解

> 字节是通过网络传输信息（或在硬盘或内存中存储信息）的单位

#### CSimpleBuffer
#### 1.简介
自己实现了一个String,内部维护一个动态数组

#### 2.接口说明
```
uchar_t*  GetBuffer()：获取改string的字符串形式，类似std::string c_str()
uint32_t GetWriteOffset()：获取已经使用的大小
void Extend(uint32_t len): 拓展len空间(realloc)，实际会再额外拓展1/4大小，如传参32会拓展到40字节
uint32_t Write(void* buf, uint32_t len): 从buf写入len字节数据
uint32_t Read(void* buf, uint32_t len): 读出len字节数据到buf
```

#### CByteStream

#### 1.简介
实现了字节数组和uint16_t int6_t uint32_t int32_t等数字的相互转换。
比如：一个uint16_t数字转换成2个字节表示的uint8_t数组

#### 2.CByteStream内部提供了许多static方法，方便外部直接使用

### 四、TeamTalk EncDec详解

> 对称加密算法加密和解密用相同的密钥  
> 高级加密标准(AES,Advanced Encryption Standard)为最常见的对称加密算法(微信小程序加密传输就是用这个加密算法的)  

#### 1.简介
>该模块依赖于UtilPdu(自己实现)和Base64(自己实现)和openssl/aes。

#### 2.主要接口说明：
#### UtilPdu
> UtilPdu接口部分前面介绍过

#### Base64
```
Base64是网络上最常见的用于传输8Bit字节码的编码方式之一，Base64就是一种基于64个可打印字符来表示二进制数据的方法  
string base64_decode(const string &ascdata);  
string base64_encode(const string &bindata);  
	用于base64编解码  
```

#### CAes
```
int Encrypt(const char* pInData, uint32_t nInLen, char** ppOutData, uint32_t& nOutLen) //加密
int Decrypt(const char* pInData, uint32_t nInLen, char** ppOutData, uint32_t& nOutLen) //解密
```

#### CMd5
```
static void MD5_Calculate (const char* pContent, unsigned int nLen,char* md5) //计算MD5(利用openssl/md5)
```
