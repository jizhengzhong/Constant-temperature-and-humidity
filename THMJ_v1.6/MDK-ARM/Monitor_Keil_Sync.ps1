# Keil 文件同步监控脚本
# 用途：监控源文件修改，提醒用户在 Keil 中刷新

Write-Host "=== Keil 文件同步监控工具 ===" -ForegroundColor Green
Write-Host "监控目录: D:\stik\MJ_DISPLAY\THMJ_v1.51github\Core\Src" -ForegroundColor Cyan
Write-Host "按 Ctrl+C 停止监控" -ForegroundColor Yellow
Write-Host ""

# 监控的目录
$watchPath = "D:\stik\MJ_DISPLAY\THMJ_v1.51github\Core\Src"

# 创建文件系统监控器
$watcher = New-Object System.IO.FileSystemWatcher
$watcher.Path = $watchPath
$watcher.Filter = "*.c"
$watcher.IncludeSubdirectories = $true
$watcher.NotifyFilter = [System.IO.NotifyFilters]::LastWrite -bor 
                        [System.IO.NotifyFilters]::FileName -bor 
                        [System.IO.NotifyFilters]::DirectoryName

# 文件修改事件
$onChange = Register-ObjectEvent $watcher "Changed" -Action {
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $fileName = $Event.SourceEventArgs.Name
    $changeType = $Event.SourceEventArgs.ChangeType
    
    Write-Host "[$timestamp] 文件已修改: $fileName" -ForegroundColor Green
    Write-Host ">>> 请在 Keil 中按 Ctrl+Tab 切换文件以触发刷新！" -ForegroundColor Yellow
    Write-Host ""
}

# 文件创建事件
$onCreated = Register-ObjectEvent $watcher "Created" -Action {
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $fileName = $Event.SourceEventArgs.Name
    
    Write-Host "[$timestamp] 新文件创建: $fileName" -ForegroundColor Cyan
    Write-Host ""
}

# 保持脚本运行
try {
    while ($true) {
        Start-Sleep -Milliseconds 500
    }
}
finally {
    # 清理
    Unregister-Event -SourceIdentifier $onChange.Name
    Unregister-Event -SourceIdentifier $onCreated.Name
    $watcher.Dispose()
    Write-Host "监控已停止" -ForegroundColor Red
}
