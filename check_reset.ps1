$port = New-Object System.IO.Ports.SerialPort COM9, 9600, None, 8, One
try {
    $port.Open()
    Write-Host "LONG LOG STARTED - PRESS BLUE BUTTON NOW..."
    $startTime = Get-Date
    while ((Get-Date) -lt $startTime.AddSeconds(20)) {
        if ($port.BytesToRead -gt 0) {
            $c = $port.ReadExisting()
            Write-Host $c -NoNewline
            if ($c -match "Booting") {
                Write-Host "`r`n[DETECTED RESET!]" -ForegroundColor Red
            }
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
