# aws-elb-demo

#目标
为Classic负载均衡器配置代理协议支持, 服务端处理代码示例

#参考文档: 
http://docs.aws.amazon.com/zh_cn/elasticloadbalancing/latest/classic/enable-proxy-protocol.htmlhttp://docs.aws.amazon.com/zh_cn/elasticloadbalancing/latest/classic/enable-proxy-protocol.html

例如: 侦听TCP 5000端口

#配置:


#测试:
客户端使用: 
1. telnet ELB_Domain_Name 5000


# 参考代码
## Python
tcp_server.py
命令: python tcp_server.py


## C
elb_tcp_server.c
编译: make
运行: ./elb_tcp_server

