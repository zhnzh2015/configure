简要介绍
--------
jcpp::configure::Configure是一个配置库，它有以下特点：
- 支持配置项分组
- 单个配置项的长度没有限制
- 使用十分方便、简洁
- 未针对多线程进行设计
- 请采用传地址和传引用的方式传递Configure对象

配置文件格式
------------
Configure的配置格式支持的数据类型有：
- 简单的Key : Value配置项
- 组配置（Group），相当于C风格结构体类型
- 数组（无下标限制）

***命名规范*** 为了严格命名规范，以增强configure的扩展性，本版的configure所有的ID（包括Key值、组名称）均只能使用 数字、大小写字母和下划线

Key-Value配置项
---------------
配置文件的基本单元是Key-Value对。中间用冒号隔开：

key : value

其中，
- key值只能是数字、大小写字母和下划线
- 一般情况下，value值仍然以行尾为结束，但增加了转义字符的概念。

转义字符：如果配置项value里面要出现引号、不可见字符、白空格等，一般是很难配置进去的，所以增加了对转义字符的支持。
如果value的第一个非空白字符是双引号，那么转义字符就会生效，value值就会以C风格的字符串进行解析。示例：

\#下面这个是普通的字符串
key1 : Hello world!
\#下面这个是一个以双引号开始的字符串，我们以C风格进行解析,\t会被解析为制表符，\x41解析为'A'，\n为回车
key2 : "\tThis is \x41 New Style!\n"

***转义字符表***

转义字符 含义           ASCII码（16/10进制）
\0       空字符(NULL)   00H/0
\n       换行符(LF)     0AH/10
\r       回车符(CR)     0DH/13
\t       水平制表符(HT) 09H/9
\v       垂直制表(VT)   0B/11
\a       响铃(BEL)      07/7
\b       退格符(BS)     08H/8
\f       换页符(FF)     0CH/12
\'       单引号         27H/39
\"       双引号         22H/34
\\       反斜杠         5CH/92
\ddd     任意字符       三位八进制
\xhh     任意字符       二位十六进制

组(Group)格式
-------------
***语法格式：***

\#使用井号注释，空行会被无视
全局Key值 : Value值

[Group1]
组1的Key值 : Value值
组1的Key值 : Value值

[Group1.SubGroup1]
SubGroup1的Key值 : Value值

\#上述SubGroup1可以简写成以下形式：
[.SubGroup2]
\#下面这个是SubGroup2的子Group，前面有两个点。它相当于[Group.SubGroup2.MoreSubGroup]
[..MoreSubGroup]
\#下面这个相当于[Group.SubGroup3]
[.SubGroup3]

[Group2]
组2的Key值 : Value值
组2的Key值 : Value值

[GLOBAL]
全局Key值 : Value值

****规则要点：***
- Group名以中括号包起来，独占一行
- Group之后的key-value对都属于该group的成员，这个是不闭合的，直达遇到下一个group为止
- [GLOBAL]字段（全大写的global）是一个关键字，表示跳转到全局区域。GLOBAL可以看做是总的group，所有group和配置项都是它的成员。
- 嵌套子Group的格式： “Group名称 + 点 + 子Group名称”，可以任意多级
- 嵌套子Group的简写形式：“若干个点 + 子Group名称”，一个点表示第一级相同，两个点表示前两级相同......

数组(Array)格式
---------------
数组格式很简单，就是在ID前面加@

下面演示了一个数组，数组名为MyKey：

@MyKey : value0
@MyKey : value1
@MyKey : value2

如果是Group的数组的话，下面演示了一个server数组的配置：

[@server]
ip : xxx
port : xxx
[@server]
ip : xxx
port : xxx

假如这个数组是属于某个group的成员，比如我的Spider模块要配多个server（这里的group名也可以写成简写形式）：

[Spider]
[.@server]
ip : xxx
port : xxx
[.@server]
ip : xxx
port : xxx

更复杂的，假如每个server有多个网卡、多个ip：

[Spider]
[.@server]
@ip : xxx
@ip : xxx
port : xxx
[.@server]
@ip : xxx
@ip : xxx
port : xxx

子配置文件
---------

特殊的：$include : "filename" 表示子配置文件
读取子配置文件时，该文件的路径统一使用load时传入的路径。每一个子配置文件，不论include出现在配置文件的什么地方，均视为GLOBAL。为避免误用，建议将所有的$include写在配置文件的最开头。

目前只允许一层include。即：子配置不能再依赖于另外的子配置。


接口文档
--------
Configure的使用【无比简单】

Configure库的位置：public/configure

头文件：Configure.h

编译依赖：

- third/glog
- third/boost

具体使用实例，见*unittest/case*下测试用例。
