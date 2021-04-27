# Avalon Error Log

### 1.执行tests/demo.py时卡在json3。发现build时start均正常但进入log阶段后有报错

**Error**: 具体error未记录，以下为github issues #61中error

```bash
[10:34:04 ERROR   __main__] failed to initialize enclave; Failed to initialize quote in enclave constructor: SGX ERROR: SGX_ERROR_BUSY
```

**解决**：重新follow PREREQUISITES.md中的Intel sgx部分，发现sdk已正常安装，重新follow 'Intel SGX in Hardware Mode'部分，确认config/singleton_enclave_config.toml中EPID与ias_api_key已修改。完成后error解决，tests/Demo.py可正常运行。

### 2. 部署avalon docker容器时报错
#### 2.1 出现solcx的问题
此问题出现在部署avalon-shell时，avalon-shell使用avalon/docker/Dockerfile，其中包括
```
python3 -m solcx.install v0.5.15
```
此句语句意在py-solc-x安装后，安装其依赖solc，可通过`>>> from solcx import install_solc >>> install_solc('v0.4.25')`安装
但此处需要将Dockerfile内此语句替换为
```
RUN add-apt-repository ppa:ethereum/ethereum
RUN apt-get update
RUN apt-get install solc
```
如此更改，删除avalon镜像后重新部署可以成功。

#### 2.2 RK_PUB.zip问题

部署期间可能出现unzip RK_PUB.zip报错的问题，这是由于RK_PUB.zip地址已更换，在common/app/verify_ias_report/build_ias_certificates_cpp.sh中将其地址改为'https://community.intel.com/legacyfs/online/drupal_files/managed/7b/de/RK_PUB.zip'即可。

### 3. 安装minifab

可以直接使用start_fabric.sh脚本进行安装，但需

在mywork文件夹下放入spec.yaml(其中proxy取消注释),其中的netname不可以改

将start_fabric.sh中tinyurl设置为MINIFAB_URL=https://tinyurl.com/yxa2q6yr

将start_fabric.sh最后一部分安装链码部分中instantiate命令改为,commit,initialize,discover

注意$TCF_HOME的值
export no_proxy=localhost,127.0.0.1,orderer3.example.com,orderer2.example.com,orderer1.example.com,peer2.org1.example.com,peer1.org1.example.com,peer2.org0.example.com,peer1.org0.example.com

### 4. 执行./generic_client.py出现的问题 （未全部解决，但此处所有问题在重新配置系统为Ubuntu18.04后均没有出现）
#### 4.1  DNS resolution failed for service: peer1.org1.example.com:7051

用```docker inspect --format='{{.Name}} - {{range.NetworkSettings.Networks}} {{.IPAddress}}{{end}}' $(docker ps -aq)```查询容器ip地址，并在avalon-shell内部安装vim，将地址写入容器内的/etc/hosts

```
172.26.0.2 peer1.org0.example.com
172.26.0.3 peer2.org0.example.com
172.26.0.4 peer1.org1.example.com
172.26.0.5 peer2.org1.example.com
172.26.0.6 orderer1.example.com
172.26.0.7 orderer2.example.com
172.26.0.8 orderer3.example.com
172.26.0.9 ca1.org0.example.com
172.26.0.10 ca1.org1.example.com
```

#### 4.2. 启动minifab以及avalon后avalon-shell无法与fabric节点通信

出现问题原因为属于不同网络的容器间默认禁止了通信，可通过下列语句查看，某容器所在网络

```
docker inspect docker-name
```

可通过下列语句查看相关通信的规则，发现其中两网络接口之间的规则为DROP，即不可通信。

```bash
sudo iptables -t filter -nvL
```

首先查看容器IP地址，通过ip地址分别找到对应的网络接口'br-xxxxxxx'。利用以下语句修改通信规则，使其能够正常通信：

```bash
sudo iptables -I DOCKER-USER -i br-818b626d02a4 -o br-6f342963b2c4 -j ACCEPT
sudo iptables -I DOCKER-USER -o br-818b626d02a4 -i br-6f342963b2c4 -j ACCEPT
```

#### 4.3 能够通信后报错有None返回值

**报错内容**

```
Traceback (most recent call last):
  File "./generic_client.py", line 390, in <module>
    Main()
  File "./generic_client.py", line 284, in Main
    worker_id
  File "/project/avalon/examples/apps/generic_client/proxy_model_generic_client.py", line 60, in get_worker_details
    worker_id
TypeError: 'NoneType' object is not iterable

```

查看代码后发现/hfc/fabric/client.py的chaincode_query有None返回值，response中有如下问题

```
2021-03-25 11:00:36,258 - INFO - Query response: [response {
  status: 500
  message: "error in simulation: failed to execute transaction 7a2a3da88e9be39b8796fed6090f502f385939310d51e9317ecc4e583c259b8c: invalid invocation: chaincode \'worker\' has not been initialized for this version, must call as init first"
}
]
```

原因为minifabric在安装链码后需要初始化(已在start_fabic.sh中添加)

```
minifab initialize -n cc_name
```

初始化后，继续报相同错误，responce有如下问题

```
2021-03-25 11:03:23,057 - INFO - Query response: [response {
  status: 500
  message: "Worker with ID \'0b03616a46ea9cf574f3f8eedc93a62c691a60dbd3783427c0243bacfe5bba94\' does not exist"
}
payload: "\n \363\337\2274\271:\324o\300\354\314\0171\241\024f\006\347A\337B\352\233?\364\355\313\222q\222\371\025\022\232\002\n\241\001\0227\n\n_lifecycle\022)\n\'\n!namespaces/fields/worker/Sequence\022\002\010\010\022f\n\006worker\022\\\n\026\n\020\000\364\217\277\277initialized\022\002\010\023\nB\n@0b03616a46ea9cf574f3f8eedc93a62c691a60dbd3783427c0243bacfe5bba94\032e\010\364\003\022`Worker with ID \'0b03616a46ea9cf574f3f8eedc93a62c691a60dbd3783427c0243bacfe5bba94\' does not exist\"\r\022\006worker\032\0031.0"
]
```

原因是singleton-worker-1未找到。

分析过程：

运行了直接模式的generic_client的echo，运行正常并能得到结果，能够查找到singleton-worker。但直接模式是从lmdb中查找worker的信息，而代理模式从区块链上读取(?)。

查看了avalon-lmdb中的数据(在容器中的Kv_Shared_tmp内，使用cat查看)，其中包含worker_id=0b03616a46ea9cf574f3f8eedc93a62c691a60dbd3783427c0243bacfe5bba94的worker，应为'singleton-worker-1'。因此直接模式下可以查询到该worker，接下来分析代理模式。

创建过程中，何时何处进行了worker的registry？

在运行generic_client时有一些创建新org、channel、peers的步骤，是否是运行genric_client才创建worker？

找到blockchain_connecter/fabric下有fabric_connecter_service以及fabric_connecter中调用的base_connecter有注册worker的函数。（将blockchain以及kv storage中的worker信息同步）


