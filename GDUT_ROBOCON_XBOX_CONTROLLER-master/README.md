
该数据帧定义用于在 ESP32 和 STM32 之间通过串口传输数据。数据帧结构主要包括帧头、帧 ID、数据长度、数据、CRC 校验码和帧尾。以下是对数据帧结构的详细总结：

数据帧结构
帧头 (frame_head)：

帧头由两个固定字节组成，FRAME_HEAD_0_MAT (0xFC) 和 FRAME_HEAD_1_MAT (0xFB)。
用于标识数据帧的开始，帮助接收端同步数据流。
帧 ID (frame_id)：

1 字节，用于标识数据帧的类型或ID。
在本示例中定义为 FRAME_ID_MAT (0x01)。
数据长度 (data_length)：

1 字节，表示数据部分的长LENGTH_MAT 定义，为 32。度。
最大数据长度由 MAX_DATA_
数据 (data)：

具体的数据载荷，可以是浮点数数组或者字节数组。
在结构体中通过 union 定义，可以以 float 数组或 uint8_t 数组的形式存储。
在数据帧中直接发送的是字节数组形式 (uint8_t)，数据长度可以为 0 到 MAX_DATA_LENGTH_MAT。
CRC 校验码 (check_code)：

2 字节，用于数据完整性校验。
通过 CRC16 校验算法生成，确保数据传输过程中不出错。
crc_code 保存了校验码，发送和接收时会比较校验码以确认数据完整性。
帧尾 (frame_end)：

帧尾由两个固定字节组成，FRAME_END_0_MAT (0xFD) 和 FRAME_END_1_MAT (0xFE)。
用于标识数据帧的结束，帮助接收端确认帧的完整性。
数据帧的发送过程
在发送函数 send_serial_frame_mat 中，首先填充数据帧的各个字段，包括帧头、帧 ID、数据长度、数据内容和帧尾。
使用 CRC16_Table 计算数据部分的校验码，并将其填入数据帧。
将完整的数据帧通过串口发送。
使用场景
该数据帧结构用于串口通信，适用于将浮点数或字节数组形式的数据从 ESP32 发送到 STM32。
数据帧设计包含头、尾、ID 和校验码，可用于高可靠性的数据传输，确保在噪声较大的环境中正确接收数据。
示例
一个典型的完整数据帧可能包含：

帧头：0xFC 0xFB
帧 ID：0x01
数据长度：8 (如果传输 2 个 float 数据)
数据：由 float 数组转换为字节数组形式的 8 字节数据
CRC 校验码：2 字节
帧尾：0xFD 0xFE
通过这种结构设计，接收端可以根据帧头和帧尾识别数据帧的起始和结束，并使用 CRC 校验码验证数据的完整性。
字节数组 xbox_datas 的结构
xbox_datas 数组共包含 28 个字节，每个字节存储手柄的不同按键状态或摇杆数据。具体定义如下：

按键状态（0~15）：

xbox_datas[0]：btnY 按钮状态，布尔值（1 为按下，0 为未按下）
xbox_datas[1]：btnB 按钮状态
xbox_datas[2]：btnA 按钮状态
xbox_datas[3]：btnX 按钮状态
xbox_datas[4]：btnShare 按钮状态
xbox_datas[5]：btnStart 按钮状态
xbox_datas[6]：btnSelect 按钮状态
xbox_datas[7]：btnXbox 按钮状态
xbox_datas[8]：btnLB 按钮状态
xbox_datas[9]：btnRB 按钮状态
xbox_datas[10]：btnLS 按钮状态
xbox_datas[11]：btnRS 按钮状态
xbox_datas[12]：方向键上（btnDirUp）状态
xbox_datas[13]：方向键左（btnDirLeft）状态
xbox_datas[14]：方向键右（btnDirRight）状态
xbox_datas[15]：方向键下（btnDirDown）状态
这些按键状态均为布尔值（1 字节），用于表示相应按键是否被按下。

摇杆和扳机值（16~27）：

xbox_datas[16] 和 xbox_datas[17]：左摇杆水平（joyLHori）值，16 位数据分为高 8 位和低 8 位
xbox_datas[18] 和 xbox_datas[19]：左摇杆垂直（joyLVert）值，16 位数据分为高 8 位和低 8 位
xbox_datas[20] 和 xbox_datas[21]：右摇杆水平（joyRHori）值，16 位数据分为高 8 位和低 8 位
xbox_datas[22] 和 xbox_datas[23]：右摇杆垂直（joyRVert）值，16 位数据分为高 8 位和低 8 位
xbox_datas[24] 和 xbox_datas[25]：左扳机（trigLT）值，16 位数据分为高 8 位和低 8 位
xbox_datas[26] 和 xbox_datas[27]：右扳机（trigRT）值，16 位数据分为高 8 位和低 8 位
这些值都是 16 位数据，被拆分为两个 8 位字节进行存储。

xbox_datas 数组的用途
该数组用于将 Xbox 手柄的按键和摇杆状态以固定格式存储，便于后续处理或发送给其他设备（如 STM32）。
数组中的前 16 个字节存储手柄的各个按键状态，后 12 个字节存储左右摇杆和左右扳机的值（16 位数值分为高低字节存储）。
通过这种固定的结构，可以快速访问和处理手柄的状态信息。
注意事项
该任务在 FreeRTOS 的核心 0 上运行，频率为 250Hz，这意味着它每隔 4 毫秒执行一次，以获取并更新 xbox_datas 数组中的数据。
在任务中，vTaskDelay 函数用于相对延时，确保任务以准确的频率运行。
通过将 Xbox 手柄数据映射到这个 xbox_datas 数组中，你可以方便地读取手柄的当前状态，并将其用于控制其他设备，如机器人运动。
