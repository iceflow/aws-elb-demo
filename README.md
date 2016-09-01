# aws-elb-demo

# 目标
为Classic负载均衡器配置代理协议支持, 服务端处理代码示例

# 参考文档: 
[为Classic负载均衡器配置代理协议支持](http://docs.aws.amazon.com/zh_cn/elasticloadbalancing/latest/classic/enable-proxy-protocol.html)


代理协议标头

在您使用为 TCP/SSL 连接配置的负载均衡器时，代理协议标头有助于识别客户端的 IP 地址。因为负载均衡器会拦截客户端与您的实例之间的流量，因此您实例的访问日志中将包含负载均衡器的 IP 地址而不是原始客户端的 IP 地址。您可以分析该请求的第一行，以检索该客户端的 IP 地址和端口号。<br>
IPv6 标头中的代理地址是负载均衡器的公有 IPv6 地址。此 IPv6 地址与从该负载均衡器以 ipv6 或 dualstack 开头的 DNS 名称解析而来的 IP 地址相匹配。如果客户端使用 IPv4 进行连接，则代理标头中的地址是该负载均衡器的私有 IPv4 地址，不能在 EC2-Classic 网络外部通过 DNS 查找进行解析。<br>
<br>
该代理协议行以回车符和换行符 ("\r\n") 结束，且具有以下形式：<br>
```Bash
PROXY_STRING + single space + INET_PROTOCOL + single space + CLIENT_IP + single space + PROXY_IP + single space + CLIENT_PORT + single space + PROXY_PORT + "\r\n"
```
示例：IPv4<br>
下面是 IPv4 的代理协议行的示例。<br>
```Bash
PROXY TCP4 54.222.152.163 10.0.11.239 45418 5000\r\n
```

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
服务端:<br>
** 接收客户端连接，处理分析第一个ELB主动发送的代理数据
** 并回应客户端的请求数据
** 收到'q'或者'Q', 断开连接
```Bash
# ./elb_tcp_server
TCPServer Waiting for client on port 5000
I got a connection from (172.31.1.237, 64724)
Client IP: 172.31.1.237
Connection (172.31.1.237, 64724): 0 received. Quit.
I got a connection from (172.31.7.103, 4986)
Client IP: 172.31.7.103
Connection (172.31.7.103, 4986): 0 received. Quit.
I got a connection from (172.31.1.237, 64729)
Client IP: 172.31.1.237
Connection (172.31.1.237, 64729): 0 received. Quit.
I got a connection from (172.31.7.103, 4990)
Client IP: 172.31.7.103
Connection (172.31.7.103, 4990): 0 received. Quit.
I got a connection from (172.31.7.103, 4991)
Client IP: 54.222.152.163
RECIEVED DATA = 123
```
客户端:<br>
Telnet 端口测试, 输入 'q' 或者 'Q' 退出连接.

```Bash
# telnet testELBProxy-*******.cn-north-1.elb.amazonaws.com.cn  5000
Trying 54.223.90.209...
Connected to testELBProxy-*****.cn-north-1.elb.amazonaws.com.cn.
Escape character is '^]'.
123
123
q
Connection closed by foreign host.
```


