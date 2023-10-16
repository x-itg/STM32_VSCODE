@echo off
chcp 65001
setlocal enabledelayedexpansion
git fetch -q origin main:fetchmain
for /f "tokens=*" %%i in ('git rev-list --count origin/main') do set "remote_count=%%i"
for /f "tokens=*" %%i in ('git rev-list --count main') do set "local_count=%%i"
IF %remote_count% gtr %local_count% (
  echo ==============================
  git log -1 --pretty=format:"%%s"
  echo 远程提交数%remote_count%大于本提交数%local_count% 
  echo 用远程仓库的内容替换本地仓库的内容
  git add .
  git checkout -f fetchmain
  git branch -D main
  git branch -m main
  git log -1 --pretty=format:"%%s"
  echo ==============================
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
    git push -q o2 main
    git push -q origin main
    git log -1 --pretty=format:"%%s"
    echo ==============================
  )
  git branch -D -q fetchmain
)