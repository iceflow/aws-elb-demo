# aws-elb-demo

# 目标
为Classic负载均衡器配置代理协议支持, 服务端处理代码示例

# 参考文档: 
[为Classic负载均衡器配置代理协议支持](http://docs.aws.amazon.com/zh_cn/elasticloadbalancing/latest/classic/enable-proxy-protocol.html)

# 配置:
* ELB配置
** 侦听TCP 5000端口
** 启用
```Bash
aws elb create-load-balancer-policy --load-balancer-name my-loadbalancer --policy-name my-ProxyProtocol-policy --policy-type-name ProxyProtocolPolicyType --policy-attributes AttributeName=ProxyProtocol,AttributeValue=true
```

# 测试:
* 客户端使用: 
1. telnet ELB_Domain_Name 5000


# 参考代码
## Python
* tcp_server.py
* 命令
```Bash
python tcp_server.py
```


## C
* elb_tcp_server.c
* 编译
```Bash
make
```
* 运行
```Bash
./elb_tcp_server
```

