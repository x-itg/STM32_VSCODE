bat
@echo off
chcp 65001
REM 检查是否有未跟踪和已修改的文件
git status | findstr /C:"Untracked files:" /C:"modified:" > nul

REM 如果同时检测到未跟踪和已修改的文件
if %errorlevel% equ 0 (
    echo 本地存在变动...............................
    git status | findstr /C:"Untracked files:" > nul
    if %errorlevel% equ 0 (
        echo 仅有已修改的文件存在
    ) else (
        echo 仅有未跟踪的文件存在
    )
    git add .
    git commit -m "Commit Untracked or Modified:%errorlevel%"
) else (
    echo 本地工作目录下文件未变更
    echo 不存在未跟踪或已修改的文件未提交的情况
)

 

REM 获取远程仓库和本地仓库的提交数量
for /f "tokens=*" %%i in ('git rev-list --count origin/main') do set "remote_count=%%i"
for /f "tokens=*" %%i in ('git rev-list --count main') do set "local_count=%%i"

REM 比较远程仓库和本地仓库的提交数量
IF %remote_count% gtr %local_count% (
  REM 远程仓库的提交数量较多，拉取远程仓库并合并到本地仓库
  git pull origin main -f
  echo 远程仓库已更新并合并到本地仓库
) else if %remote_count% lss %local_count% (
  REM 本地仓库的提交数量较多，推送本地仓库到远程仓库
  git commit -am "count:%local_count%@%COMPUTERNAME%"
  git push origin main
  echo 本地仓库已更新并推送到远程仓库
) else (
  REM 远程仓库和本地仓库的提交数量相同，无需操作
  echo 远程仓库和本地仓库的提交数量相同，无需操作
)



pause