$port = New-Object System.IO.Ports.SerialPort COM9, 9600, None, 8, One
try {
    $port.Open()
    Write-Host "LOGGING STARTED - PLEASE PRESS REMOTE BUTTON NOW..."
    $startTime = Get-Date
    while ((Get-Date) -lt $startTime.AddSeconds(10)) {
        if ($port.BytesToRead -gt 0) {
            Write-Host $port.ReadExisting() -NoNewline
        }
        Start-Sleep -Milliseconds 100
    }
}
catch {
    Write-Host "Error: $_"
}
finally {
    $port.Close()
    Write-Host "`r`nLOGGING FINISHED."
}
