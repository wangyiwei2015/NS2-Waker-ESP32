# NS2唤醒器-基于ESP32
使用Arduino IDE

按下BOOT按钮即可发送唤醒包

## 抓包说明

### 工具

nRF Connect或者LightBlue等App都可以

注意：iOS由于系统限制，无法获得扫描设备MAC，推荐使用Android最方便

### 找到Joy-Con设备

Joy-Con 2 的 MAC 地址：

```
78:81:8C:XX:XX:XX
```

其中`78:81:8C`是了Nintendo注册的MAC前缀（据说还有更多别的），因此JC应该由此开头。

找出信号最强的符合条件的设备

### 获取厂商广播数据包

广播类似如下形式：

```
02 01 06 1B FF 53 05 …
```

需要找到并替换进代码中。

### 编译和使用

两个版本

- 常规NS2_Waker：按钮触发唤醒一次，平时待机，逻辑更容易理解，可以集成其他功能。
- 硬件睡眠NS2_Waker_sleepy：利用ESP的睡眠功能，每次启动就唤醒一下，然后睡眠，可以用RST按钮来触发唤醒功能；功耗低。

---

#### Vibe-Coding说明

使用了 Gemini 3.5 Pro
