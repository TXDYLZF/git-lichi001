# Linux 常用命令笔记（一）：date 与 find

## 一、date：查看和设置系统时间

```bash
date                                  # 查看当前时间
sudo date -s "1999-01-01 15:30:25"    # 设置时间（必须 sudo）
date +"%Y-%m-%d %H:%M:%S"             # 按指定格式输出
```

要点：

- `date` 不带参数是**读**，任何用户都能用；`-s` 是**写**，必须 root。
- 系统时钟（内核维护）与硬件时钟（BIOS）是两个东西，改完要对齐：

  ```bash
  sudo hwclock -w      # 系统时间 → 硬件时钟
  sudo hwclock -r      # 读硬件时钟
  ```

- 时区不是用 `timedatectl` 改，而是改软链接（本实验虚机没有 systemd，`timedatectl` 不存在）：

  ```bash
  sudo ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
  date                 # 应显示 CST / +0800
  ```

- 嵌入式板子上通常也没有 systemd，所以「改 `/etc/localtime` + `date -s` + `hwclock`」这套是通用做法。

---

## 二、find 的语法骨架

```
find   搜索路径    匹配条件    执行操作
       ↑           ↑          ↑
       从哪开始找   筛什么      找到后干啥
```

| 位置 | 说明 | 例子 |
|---|---|---|
| 搜索路径 | 不写默认是 `.`（当前目录），可写多个 | `find /etc /var -name x` |
| 匹配条件 | 名字、类型、时间、权限、大小、属主，可叠加（默认与） | `-name "*.log" -type f -size +10M` |
| 执行操作 | 不写默认 `-print` | `-ls`、`-delete`、`-exec` |

注意：搜索路径越大越慢，`find /` 会翻遍 `/proc`、`/sys` 并报权限错误，记得 `2>/dev/null`。

```bash
find / -name "*.conf" 2>/dev/null     # 全盘，慢
find /etc -name "*.conf"              # 只翻 /etc，快
```

## 三、-name 按名字查找

```bash
find . -name "file1.txt"
find . -iname "*.TXT"           # 忽略大小写
find . -maxdepth 2 -name "*.c"  # 限制搜索深度
```

- 支持通配符 `*` `?` `[]`，**通配符必须加引号**，否则被 Shell 先展开。
- `-name` 匹配的是**文件名（含软链接自身）**，所以要常和 `-type` 搭配。

## 四、-type 按类型查找

```bash
find /var/log -type f -name "*.log"
```

| 字母 | 类型 | `ls -l` 首字符 | 例子 |
|---|---|---|---|
| `f` | 普通文件 | `-` | `.c`、`.txt`、`/etc/passwd` |
| `d` | 目录 | `d` | `/home`、`/etc/rc1.d` |
| `l` | 符号链接 | `l` | `/etc/mtab -> /proc/mounts` |
| `b` | 块设备 | `b` | `/dev/sda`、`/dev/mmcblk0` |
| `c` | 字符设备 | `c` | `/dev/ttyS0`、`/dev/null` |

```bash
find /etc -type l            # 软链接
find /dev -type b            # 块设备（磁盘）
find /dev -type c            # 字符设备（串口等）
```

## 五、时间条件：-mtime / -atime / -ctime / -mmin

| 条件 | 含义 | 记法 |
|---|---|---|
| `-mtime` | 文件**内容**最后修改（天） | 改内容 |
| `-atime` | 文件最后被**读取**（天） | 被看过 |
| `-ctime` | **inode 状态**变更（天） | 改属性，不是改内容 |
| `-mmin` | 同 mtime，单位**分钟** | 精细版 |

正负号是重点：

| 写法 | 含义 |
|---|---|
| `-mtime -1` | 1 天内修改过（最近） |
| `-mtime +1` | 1 天以前修改过（更早） |

```bash
find . -mtime -1                    # 最近 1 天内改过
find . -mmin +30                    # 30 分钟前改过
find . -mmin -10                    # 最近 10 分钟内改过
find ~/clab -mmin -30 -type f       # 半小时内动过的源码
find /var/log -mtime +7 -name "*.log"   # 7 天以上的老日志
```

## 六、-perm 按权限查找

```bash
find . -perm 644       # 精确匹配 644
find . -perm -644      # 至少包含这些位
find . -perm /644      # 任意一位命中即可
```

实战用途（比查 644 更有意义）：

```bash
find / -perm -4000 -type f 2>/dev/null   # 找 SUID 程序（提权排查）
find . -perm -o=w -type f                # 找其他人可写的文件（安全隐患）
```

## 七、查找后执行操作

```bash
find /etc -type l -ls                        # 打印详情
find . -name "*.bak" -exec rm -f {} \;       # 对每个结果执行命令
find . -name "*.tmp" -delete                 # find 自带删除
find . -name "*.log" -ok rm {} \;            # 交互式，逐条确认
find . -name "*.c" -exec wc -l {} +          # 用 + 一次传多个文件
```

- `{}` 是占位符，代表当前找到的文件名。
- `\;` 是结束标记，分号要转义。
- ⚠️ `-delete`、`-exec rm` **不可逆**，先去掉执行动作跑一遍确认清单。

## 八、速查表

```bash
find <路径> -name "模式"            # 按名字
find <路径> -iname "模式"           # 按名字，忽略大小写
find <路径> -type f|d|l|b|c        # 按类型
find <路径> -mtime -N / +N         # 修改时间（天）
find <路径> -mmin -N / +N          # 修改时间（分钟）
find <路径> -perm -644 / -4000     # 按权限
find <路径> -user softeem          # 按属主
find <路径> -size +10M             # 按大小
find <路径> ... -ls / -delete / -exec CMD {} \;
```

嵌入式常用三条：

```bash
find /dev -type c -name "tty*"          # 找可用串口
find / -size +10M -type f 2>/dev/null   # 找大文件，rootfs 瘦身
find /lib/modules -name "*.ko"          # 找内核模块
```

## 九、踩坑记录

- `find -name"cal"` 报 `unknown predicate`：**选项和值之间必须有空格**，且第一个参数是路径，省略要写 `.`。
- 文件名**区分大小写**，`s02single` 和 `S02single` 是两个文件。
- 输出末尾的 `Permission denied` 不是错误，只是没权限进某些目录，用 `2>/dev/null` 屏蔽。

## 十、关机与重启：shutdown 家族

```bash
shutdown -h now          # 立即关机（halt）
shutdown -h 10:30        # 定时：10:30 关机
shutdown -r +30          # 30 分钟后重启（reboot）
shutdown -c              # 反悔：取消已计划的关机
```

| 命令 | 等价写法 | 说明 |
|------|---------|------|
| `shutdown -h` | `halt` / `poweroff` | 停机（poweroff 顺带断电） |
| `shutdown -r` | `reboot` | 重启 |

**为什么教学首选 shutdown**：它走 systemd/init 的正常流程，**先通知所有在线用户、按序停服务再断电**；halt/poweroff/reboot 是直接动作。多用户系统里突然 poweroff 会砍掉别人的会话。

**timedatectl 补充**（有 systemd 的系统）：

```bash
timedatectl status                      # 看时间/时区/NTP 状态
timedatectl set-timezone Asia/Shanghai  # 改时区（注意拼写，不是 ShangHai）
```

本实验虚机（Ubuntu 16.04 精简版）**没有 systemd**，timedatectl 不存在——改时区用老办法：
`sudo ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime`（详见第一节的 date 部分）。

**嵌入式视角**：嵌入式板子上往往连 shutdown 都没有，断电就是断电——所以重要数据要落盘（fsync）、文件系统选掉电安全的（或加只读保护），这正是 XFS/ext4 日志和 ubifs 存在的理由。
