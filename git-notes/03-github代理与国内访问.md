# GitHub 国内访问与代理配置

> 适用场景：在国内直连 `github.com` 网页超时、git push 卡住时。
> 核心结论：**网页超时 = 没走代理**；git 要单独配代理，且 SSH 方式吃不到 `http.proxy`，需切 HTTPS。

---

## 一、先判断：代理起了没有（诊断）

Clash Verge 等工具的本质是在本机开一个**本地代理端口**，所有流量经它转发到境外再访问 GitHub。

### 1. 查代理进程与端口（Windows）

```bash
# 看 clash 内核是否在跑
tasklist | grep -i -E 'clash|verge|mihomo'

# 看本机哪些端口在监听（找代理端口，常见 7890/7893/7897）
netstat -ano | grep LISTEN | grep -E '127.0.0.1:'
```

- `clash-verge.exe` + `verge-mihomo.exe` 在跑 → 工具已开
- `verge-mihomo.exe` 监听的 `127.0.0.1:XXXX` 就是**代理端口**
- 本机实测：**Clash Verge 的 mixed 端口是 `7897`**（不是默认 7890，以 netstat 实际监听为准）

### 2. 测代理能不能连上 GitHub

```bash
# 把 7897 换成你实际监听的端口
curl -s -o /dev/null --max-time 5 -x http://127.0.0.1:7897 https://api.github.com && echo "代理可用" || echo "代理不可用"
```

> 注意：代理端口「在监听」≠「能连外网」。如果内核起来了但没选节点，端口在听但 curl 会失败——**需要在 Clash Verge 里选一个可用节点**。

---

## 二、让 git 走代理（关键步骤）

`http.proxy` **只对 HTTPS 远端生效，对 SSH 远端不生效**。所以两件事一起做：

### 1. 配置 git 全局代理

```bash
git config --global http.proxy  http://127.0.0.1:7897
git config --global https.proxy http://127.0.0.1:7897

# 想免密推送（HTTPS 方式用 token 登录，记一次后免填）
git config --global credential.helper manager
```

### 2. remote 从 SSH 切到 HTTPS

```bash
git remote set-url origin https://github.com/用户名/仓库名.git
git remote -v        # 确认已变成 https://
```

### 3. 验证 git 走代理是否通

```bash
# 用公开仓库测，不需密码
git ls-remote https://github.com/git/git HEAD
# 能返回一串 SHA 就说明代理下 git 通了
```

### 4. 推送

```bash
git push
# 首次会弹登录：用户名填 GitHub 用户名，密码填 Personal Access Token（不是账号密码）
```

> 🔑 Token 获取：GitHub 网页 → Settings → Developer settings → Personal access tokens → Tokens (classic) → Generate new token (classic) → 勾 `repo` → 生成后**立刻复制保存**。

---

## 三、让浏览器能开 github.com（别忘了这步）

git 配好代理 ≠ 浏览器也能上。Clash Verge **默认不接管系统流量**，必须手动开开关：

1. **Proxies 标签页**：确认选了一个可用节点（或选 `Auto` 自动测速）。
2. **打开「System Proxy / 系统代理」开关** —— 让 Windows 把浏览器流量指向 `127.0.0.1:7897`，**这是网页能打开的关键**。
3. 还打不开 → 把代理模式从 `Rule` 切到 **`Global`（全局）**。
4. （可选）开 **TUN 模式**，连命令行/其他程序都强制走代理。

验证：浏览器无痕窗口打开 `https://github.com`，能正常加载即生效。

---

## 四、不用代理的合法替代：Gitee（码云）

如果没代理工具，可用国内平台 Gitee 当主用，GitHub 当备份：

```bash
cd /d/github-work/git-lichi001
git remote add gitee https://gitee.com/你的用户名/仓库名.git
git push gitee main        # 推到 Gitee，国内访问飞快
```

---

## 五、git push 时好时坏？重试循环

SSH 直连（不开代理时）经常抽风，用循环多试几次：

```bash
for i in 1 2 3 4 5; do echo "=== 第 $i 次 ==="; timeout 40 git push && break; sleep 3; done
```

---

## 六、收尾：不需要代理时关掉

```bash
git config --global --unset http.proxy
git config --global --unset https.proxy
```
