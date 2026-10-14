# 13 git 工程实践（大白话版）

> 来源：QQ `12.md`（git 本地操作 / 日志 / 版本切换）
> 这不是 C 语言，但课堂笔记里有，且你本机就在用 git 管理 `c-notes`，所以一并讲透。

git = **版本控制器**：给你两样东西——**团队合作**（多人改同一份代码不打架）和**后悔药**（改错了能回到过去任一版本）。

---

## 1. 先认自己（一次性配置）

```bash
git config --global user.name  "你的名字"
git config --global user.email "你的邮箱"
```

---

## 2. 换行符（Windows 坑）

Windows 换行是 `\r\n`，Linux/Mac 是 `\n`，混用会让 git 整天报「文件变了」其实只是换行不同。

```bash
git config --global core.autocrlf input   # 提交时 \r\n → \n；检出不转（推荐 Windows 开发）
# 或 true：检出时 \n → \r\n；或 false：完全不管
```

> 你本机 `git-lichi001` 之前出现过 `LF will be replaced by CRLF` 的提示，就是这个，无害。

---

## 3. 日常四步：init → add → commit → status

```bash
git init              # 在当前目录建仓库（只需一次）
git add file.c        # 把 file.c 加入「暂存区」（准备提交）
git add .             # 加当前目录所有改动
git status            # 看现在有什么没提交（最常敲的命令）
git commit -m "说明"  # 把暂存区的内容提交，生成一个新的版本
```

- **一定要写 `-m` 说明**：以后回看才知道这版干了啥。
- `git commit -a -m "x"`：`-a` = 自动 add 已被跟踪的修改文件（对**新增的未跟踪文件无效**）。
- 想撤销暂存：`git restore --staged file.c`（老写法 `git rm --cached file.c`）。

---

## 4. 看历史 / 后悔药

```bash
git log                  # 完整历史（作者/日期/说明）
git log --pretty=oneline # 一行一条，干净
git reflog               # 所有「动作」记录（含 reset），后悔药中的后悔药
```

**回到过去**：

```bash
git reset --hard 6a65f6d        # 回到某次提交的版本（commit 哈希前几位即可）
git reset --hard HEAD@{3}       # 回到 reflog 里第 3 条动作之前的样子
```

⚠️ `reset --hard` 会**丢掉工作区未提交的改动**，用前确认没东西要留。

---

## 5. 删除文件

```bash
rm file.c              # 先从磁盘删
git add file.c         # 再告诉 git（或 git rm file.c 一步到位）
git commit -m "删了file"
```

---

## 6. 一张图：三个区

```
工作区(你编辑的文件)  ──git add──▶  暂存区(准备提交)  ──git commit──▶  版本库(永久记录)
      ↑ restore                                  ↑ reset --hard
      └──────────────────────────────────────────────────────────────┘
```

---

## 你本机实际用的（顺带记一笔）

- 仓库 `D:\github-work\git-lichi001`，双远程：`gitee`（SSH 免密）、`origin`（GitHub HTTPS）。
- 推送：`git push gitee main` / `git push origin main`。
- `c-notes/code/output/` 已被 `.gitignore` 忽略，编译产物不进仓库。

---

## 自测
- git 给你哪两样东西？→ 团队合作 + 后悔药（版本回退）
- `git add .` 和 `git commit -a` 区别？→ add . 加所有（含新文件）；-a 只自动加已跟踪的修改
- `reset --hard` 危险在哪？→ 丢掉未提交的工作区改动
- Windows 换行符坑怎么解？→ `core.autocrlf input`
