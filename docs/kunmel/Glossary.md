# Glossary

###  Enclave Avalon Worker

####  	KME + WPE

KME: 密钥管理，负责密钥管理以及控制WPE执行work order。

WPE: 执行请求，无法访问密钥且需要KME提供相关上下文。

#### 	Singleton

Avalon 的默认worker类型，同时管理密钥以及工作负载。

### graphene

一个库操作系统(或“LibOS”)，它在用户空间库中提供一个操作系统环境来执行应用程序。它用于执行在TEE中未修改的代码。

### Trusted compute service (TCS)

提供可信计算功能的服务。Avalon算作一种TCS。





