[点击下载](https://github.com/arifeng/CharsetAssistant/blob/master/Release/CharsetAssistant.exe?raw=true) 32位版本，适用于Windows XP及之后的操作系统。

## 使用说明

本程序支持拖拽操作，可直接将文件[夹]拖拽到相应位置。

### 编码查询

查询完一种编码后可直接选择其他编码进行查询，如需更换查询的文本点击“清空”即可。可将查询结果保存为rtf格式文件。示例如下：

查询GBK编码：

![GBK](https://github.com/arifeng/CharsetAssistant/blob/master/screenshots/charset_query_gbk.png)

查询UTF-8编码：

![UTF-8](https://github.com/arifeng/CharsetAssistant/blob/master/screenshots/charset_query_utf8.png)

查询UTF-16LE (Unicode)编码：

![UTF-16LE](https://github.com/arifeng/CharsetAssistant/blob/master/screenshots/charset_query_utf16le.png)

查询汉字区位码：

![区位码](https://github.com/arifeng/CharsetAssistant/blob/master/screenshots/charset_query_quwei.png)

查询五笔字型码：

![五笔字型](https://github.com/arifeng/CharsetAssistant/blob/master/screenshots/charset_query_wubi.png)

### 编码转换

支持转换单个文件或整个文件夹

![Convert](https://github.com/arifeng/CharsetAssistant/blob/master/screenshots/charset_converter.png)

### 编码检测

支持检测单个文件或十六进制编码序列

![Detect](https://github.com/arifeng/CharsetAssistant/blob/master/screenshots/charset_detection.png)

### 十六进制查看

![HexView](https://github.com/arifeng/CharsetAssistant/blob/master/screenshots/hex_viewer.png)

## 编译方法

本程序使用 VC6 构建，需要同步子模块

    git submodule init
    git submodule update

然后双击 CharsetAssist.dsw 打开解决方案进行编译。
