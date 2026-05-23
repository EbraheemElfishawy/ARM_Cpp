$port = New-Object System.IO.Ports.SerialPort COM9, 9600, None, 8, One
try {
    $port.Open()
    $port.RtsEnable = $true
    $port.DtrEnable = $true
    Write-Host "FINAL LONG MONITOR (120s) - STARTING NOW..."
    $startTime = Get-Date
    while ((Get-Date) -lt $startTime.AddSeconds(120)) {
        if ($port.BytesToRead -gt 0) {
            Write-Host $port.ReadExisting() -NoNewline
        }
        Start-Sleep -Milliseconds 100
    }
}
finally {
    $port.Close()
}
