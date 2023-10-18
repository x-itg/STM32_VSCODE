bat
@echo off
chcp 65001
setlocal enabledelayedexpansion
git fetch -q origin master:fetchmaster
for /f "tokens=*" %%i in ('git rev-list --count origin/master') do set "remote_count=%%i"
for /f "tokens=*" %%i in ('git rev-list --count master') do set "local_count=%%i"
IF %remote_count% gtr %local_count% (
  echo ==============================
  git log -1 --pretty=format:"%%s"
  echo 远程仓库的提交数量较多，拉取
  echo ==============================
  git pull origin master -f
  echo echo 通过拉取远程仓库保持同步
  git log -1 --pretty=format:"%%s"
  echo ------------------------------
)else  (
  echo ==============================
  git log -1 --pretty=format:"%%s"
  set /a Change=0
  git status | findstr /C:"Untracked files:"> nul
  if not errorlevel 1 (
    set Change=1
    echo 存在未跟踪的文件 准备重新添加推送
  )  
  git status | findstr /C:"modified:"> nul
  if not errorlevel 1 (
    set Change=1
    echo 存在修改的文件 准备推送 !Change! 
  ) 
  git status | findstr /C:"deleted:"> nul
  if not errorlevel 1 (
    set Change=1
    echo 存在删除的文件 准备推送 !Change! 
  )
  if "!Change!"=="0" (
    echo 本地仓库无变化，不推送 !Change!
    git log -1 --pretty=format:"%%s"
    echo ==============================
  )
  if "!Change!"=="1" (
    echo 本地仓库有变化，推送 !Change!
    git add .
    git commit -am "count:%local_count%@%COMPUTERNAME%"
    git push -q origin master
    git log -1 --pretty=format:"%%s"
    echo ==============================

  )
  
)
git branch -D -q fetchmaster
pause