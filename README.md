# git-lichi001

Linux 学习笔记与实验代码存档。

## 目录结构

```
git-lichi001/
├── README.md               本文件
├── GIT-CHEATSHEET.md       Git 命令速查表
├── linux-notes/            Linux 课程笔记
│   ├── 01-常用命令与find.md
│   └── 02-权限与软链接.md
└── lab/                    实验代码（从实验虚机 scp 拉回）
```

> 想让结构更整齐，可以把速查表移进 `git-notes/`：
> `mkdir git-notes && git mv GIT-CHEATSHEET.md git-notes/`

## 环境说明

- 笔记用 Typora 编写，Markdown 格式。
- 实验环境：远程实验虚机 Ubuntu 16.04（`softeem@192.168.2.129`），
  通过 `~/.ssh/config` 里的 `ubuntu-vm` 别名访问。
- 所有 git 操作在本机 Windows 上完成，虚机只用于跑实验。

## 工作流

1. 虚机里写代码、跑通实验
2. 需要留档的代码用 `scp` 拉回本机：

   ```bash
   scp -r ubuntu-vm:~/clab ./lab/
   ```

3. 笔记直接在仓库目录里用 Typora 编写
4. 提交并推送：

   ```bash
   git status
   git add .
   git commit -m "add: 说明"
   git push
   ```

## 提交信息约定

| 前缀 | 用于 |
|---|---|
| `add:` | 新增笔记或文件 |
| `fix:` | 修正错误内容 |
| `update:` | 更新已有内容 |
