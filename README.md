# Multipoint Transmission based on nrf24l01

基于nrf24l01的多点传输，传输温度数据，温度数据通过 DS18B20 采集

## nrf24l01 的一些特性

- 最大支持六个发送节点通过六个不同的数据通道同时传输数据给接收节点
- 其他特性请查看 datasheet

## 发送节点

- 发送节点1 Sending_node1_Pipe0 使用 nrf24l01 的数据通道0进行数据传输

- 发送节点2 Sending_node2_Pipe1 使用 nrf24l01 的数据通道1进行数据传输

- 发送节点3 Sending_node3_Pipe2 使用 nrf24l01 的数据通道2进行数据传输

- 发送节点4 Sending_node4_Pipe3 使用 nrf24l01 的数据通道4进行数据传输
- ~~发送节点5，使用 nrf24l01 的数据通道5进行数据传输，请自行制作~~
- ~~发送节点6，使用 nrf24l01 的数据通道6进行数据传输，请自行制作~~

## 接收节点

- 接收节点A Receiving_nodeA_LCD1602 可以接收以上四个发送节点的数据，通过 LCD1602 显示
- 接收节点B Receiving_nodeB_LCD12864 可以接收以上四个发送节点的数据，通过 LCD12864 显示
- 接收节点C Receiving_nodeC_LCD240128 可以接收以上四个发送节点的数据，通过 LCD240128 显示
- 以上三个接收节点可以同时接收以上四个发送节点的数据，也可以单独工作，只是显示的 LCD 不一样而已

## 注意事项

- 所有发送节点和接收节点是基于 STC89C52RC 单片机开发的
- 由于 nrf24l01 模块需要使用 SPI 总线控制和访问数据，而51单片机没有片上 SPI 资源，故使用软件 SPI
- 使用 Keil 5 搭建的工程
- 以上程序仅提供参考，有问题请提 issue

## 维护人信息

[WillianChan](https://github.com/willianchanlovegithub)

