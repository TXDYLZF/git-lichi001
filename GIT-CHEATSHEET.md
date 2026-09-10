# Git / GitHub 速查表

> 环境：Windows Git Bash + RHEL 8（实验机）｜仓库统一放 `D:\github-work`
> 身份：lichi / 1945664784@qq.com（邮箱必须和 GitHub 注册邮箱一致）

---

## 一、日常四步循环（90% 的日常）

```bash
git status                    # 看改了什么（随手敲，最常用）
git add 文件名                 # 加入暂存区（git add . = 全部）
git commit -m "说明改了啥"     # 提交到本地仓库
git push                      # 推到 GitHub
git pull                      # 干活前先拉最新（多机/多人必做）
```

记忆：**改 → add → commit → push**，四步一套。

---

## 二、建一个新仓库（标准流程）

1. GitHub 网页 → New repository → 填名字 → **不要勾** README/.gitignore/license（本地已有内容时勾了会冲突）
2. 本地：

```bash
cd /d/github-work
mkdir 仓库名 && cd 仓库名
git init
# 把文件放进来，先建 .gitignore
git add .
git commit -m "first commit"
git branch -M main
git remote add origin git@github.com:你的用户名/仓库名.git
git push -u origin main        # 第一次推要带 -u，以后只敲 git push
```

3. 或者反过来，网页上先建好再拉：

```bash
cd /d/github-work
git clone git@github.com:你的用户名/仓库名.git
```

---

## 三、看历史 / 看改动

```bash
git log --oneline --graph     # 简洁历史（一行一条 + 分支图）
git log -p 文件名              # 某文件每次改动详情
git diff                      # 工作区还没 add 的改动
git diff --staged             # 已 add、还没 commit 的改动
git show                      # 最近一次提交改了什么
```

---

## 四、撤销（⚠️ 先看警告）

```bash
git restore 文件名              # 丢弃工作区改动（未 add）⚠️ 不可恢复
git restore --staged 文件名    # 撤销 add（改动还在，安全）
git commit --amend             # 修改上一次的提交说明/内容（已 push 就别用）
git revert 提交id              # 生成一条"反向提交"抵消某次提交 ✅ 安全
git reset --soft HEAD~1        # 撤销上次 commit，改动保留（安全常用）
```

> ⚠️ **危险区**：
> - `git reset --hard`：**彻底丢弃**改动，找不回来。敲之前必须先 `git status`。
> - `git push -f`：覆盖远程历史，只在自己一个人的仓库用。
> - `git checkout -- .`：同 reset --hard，全部丢弃。

---

## 五、分支

```bash
git branch                     # 看所有分支
git checkout -b 新分支名        # 开新分支并切过去
git checkout main              # 切回 main
git branch -d 分支名            # 删分支（先切走才能删）
git merge 分支名                # 把指定分支合进当前分支
```

协作姿势：改代码前开分支 → push 分支 → GitHub 上发 PR → 合并 → 本地 `git pull`。

---

## 六、紧急救场

```bash
git stash                      # 手头改到一半，先藏起来
git stash pop                  # 回来继续（取出来）
git stash list                 # 看藏了哪些
```

---

## 七、远程相关

```bash
git remote -v                  # 看这个仓库连着哪个 GitHub 地址
git remote set-url origin git@github.com:新地址.git   # 换远程地址
git push -u origin main        # 首次推送并建立关联
```

---

## 八、.gitignore（仓库根目录，建仓库第一天就配）

```gitignore
# C / 嵌入式通用
*.o
*.elf
*.bin
*.hex
*.a
build/
output/

# 编辑器 / 系统
.vscode/
.idea/
*.swp
*~
Thumbs.db
.DS_Store

# 日志与临时文件
*.log
*.tmp
```

> 注意：.gitignore 只对**还没被跟踪**的文件生效。已提交过的要先 `git rm --cached 文件名` 再提交。

---

## 九、以后会用到（嵌入式方向）

| 命令 | 用途 |
|---|---|
| `git tag v1.0` + `git push --tags` | 给固件发版打标记 |
| `git submodule add <url>` | 引入第三方库/SDK（内核模块练习常用） |
| `git bisect` | 二分定位哪次提交引入了 bug |
| `git clone --depth 1 <url>` | 只拉最新版本，大仓库（如内核源码）省 80% 空间 |
| `git blame 文件名` | 看每行代码是谁哪次提交写的 |

---

## 十、常见报错对照

| 报错 | 原因 | 解决 |
|---|---|---|
| `Permission denied (publickey)` | SSH key 没配好 | 重跑 `ssh -T git@github.com` 检查；确认公钥已贴到 GitHub |
| `failed to push some refs` | 远程有你没有的提交 | 先 `git pull` 再 `git push` |
| `not a git repository` | 当前目录没初始化过 | 确认在仓库目录里；或先 `git init` |
| `$'\r': command not found`（跑 .sh 时） | 脚本是 Windows 换行符 | `dos2unix 文件名`（RHEL 8: `sudo dnf install dos2unix`） |
| `Please tell me who you are` | name/email 没配 | 重跑开头的两条 `git config --global` |
