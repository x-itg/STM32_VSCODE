bat
@echo off
chcp 65001
git fetch o2 master:fetchmaster
for /f "tokens=*" %%i in ('git rev-list --count o2/master') do set "remote_count=%%i"
for /f "tokens=*" %%i in ('git rev-list --count master') do set "local_count=%%i"
IF %remote_count% gtr %local_count% (
  echo ==============================
  echo 远程仓库的提交数量较多，拉取
  echo ==============================
  git pull o2 master -f
  echo echo 通过拉取远程仓库保持同步
  echo ------------------------------
)else  (
  echo ==============================
  echo 以本地文件为准，推送
  echo ==============================
  git add .
  git commit -am "count:%local_count%@%COMPUTERNAME%"
  git push o2 master
)
git branch -D fetchmaster
pause