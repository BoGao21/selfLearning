一. Understand-Anythiny离线安装部署
1. 更新node

(1)网络不受限
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.1/install.sh | bash
source ~/.bashrc   # 或 source ~/.zshrc

nvm install 22
nvm use 22
nvm alias default 22

node -v   # 应输出 v22.x.x

(2)网络受限
curl -fsSL https://deb.nodesource.com/setup_22.x | sudo -E bash -
sudo apt-get install -y nodejs

2.下载源码
git clone https://github.com/Egonex-AI/Understand-Anything.git
后者直接zip包下载解压也可以
并且进入目录
cd Understand-Anything

3.安装依赖与构建
npm config set registry https://registry.npmmirror.com
npm install -g pnpm
pnpm install
pnpm build

4.安装到Agent中
(1)claude code
在线方式
/plugin marketplace add Egonex-AI/Understand-Anything
/plugin install understand-anything
离线方式（对应之前源码编译understand-anything）
a.将本地目录添加为插件市场
/plugin marketplace add /root/Understand-Anything

b.安装
/plugin install understand-anything@understand-anything


(2)其他平台
Understand-Anything目录下有一个install.sh，可以安装到以下Agent中
1) gemini
2) codex
3) opencode
4) pi
5) openclaw
6) antigravity
7) vibe
8) vscode
9) hermes
10) cline
11) kimi
12) trae
13) nanobot
14) kiro

5.使用
(1) 分析代码库，生成知识图谱
/understand

(2)中文支持
/understand --language zh

(3)打开仪表盘
/understand-dashboard

(4)其他命令
命令	                      作用
/understand-chat <问题>	    基于已生成的图谱，用自然语言提问，如“鉴权流程是怎么跑的？”
/understand-diff	          在提交代码前运行，分析当前修改会波及/影响哪些其他模块，提前预防 Bug
/understand-explain         <文件/函数>	深入理解某个特定文件或函数的细节
/understand-onboard	        为团队新成员生成引导式上手文档
/understand-domain	        提取业务领域知识（领域、流程、步骤），将代码映射到业务流程

(5)文件
文件	                                      说明
.ua/knowledge-graph.json	                  新项目的默认图谱文件位置
.understand-anything/knowledge-graph.json	  兼容旧版项目的图谱文件位置

(6)如果你不想通过 AI 对话启动面板，也可以在终端直接运行以下命令，然后手动访问 http://localhost:3000
ua serve




