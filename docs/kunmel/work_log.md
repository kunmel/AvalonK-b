## 2021.4.27 

**希望实现的功能：编写workload(c++) --》 用openssl加密 --》 放到enclave-manager --》 通过脚本调用**
* 发现不能直接按教程部署多个workload的问题，解决
* 推测加密文件使用长度为32的ByteArray+AES256进行加密
* 原代码中使用了大量密码学的东西，看了半天但是感觉没法用
* 想用python，但原文中使用的个是c++与openssl，使用openssl，安装
* 使用openssl除了需要安装，还需要链接动态库，通过下列语句链接库并生成可执行文件：
	```
	g++ filename.cpp -L ~/openssl -l ssl -l crypto
	```
* 目前能够生成密钥，下一步实现加密
* 源代码中实现的方法是先提交请求，生成文件并返回key，再次发送key来读取信息
## 2021.4.28
* 确定加密方法是否是AES256
* 确定了使用的是AES-GCM-256加密方法，希望用common/app/crypto下的库进行加密，加密后传入进行测试
* 使用avalon/common/cpp提供的函数编写了加密的脚本,但不知道怎么去编译
* 尝试将原Cmake文件中没有用的部分注释后cmake&&make,但报错stdio.h: 没有那个文件或目录
* X 尝试删除gcc并重新安装来解决,重新配置了但没有用
* 按照https://www.cnblogs.com/wuchaodzxx/p/8916009.html重新编写了CMakeLists文件
* 编写后能够正常cmake,但make时有写函数报错未定义的引用,其中opensssl的函数问题可用以下命令解决
```
find_package(OpenSSL REQUIRED)
target_link_libraries(Demo OpenSSL::SSL)
```
* 目前仍残留部分未定义的引用问题
* 发现这种方法好像无法加密可执行文件,只能加密文本文件,