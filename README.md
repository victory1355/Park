# 智能小轿车停车场计费系统的设计与实现

- 功能
  - 实时视频聊天功能；
 - 实现实时聊天功能；
 - 实现多用户实时聊天功能。


- 方案
  - 采用LCD触摸屏作为系统人机交互的方式，系统运行后，创建3条线程分别负责处理用户视频数据、音频数据、人机交互，通过信号量、互斥锁等方式对线程进行同步与互斥。
  - 通过Linux的socket网络编程实现音视频数据的传输，最终显示到用户的LCD屏幕上

涉及的技术：JPEG的编解码，V4L2库、ASLA框架、Linux多线程编程、网络编程、音视频编程、信号、消息队列等IPC


注：效果演示图未上传
