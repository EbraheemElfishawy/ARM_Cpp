$port = New-Object System.IO.Ports.SerialPort COM9, 9600, None, 8, One
try {
    $port.Open()
    Write-Host "Reading COM9..."
    Start-Sleep -Seconds 5
    $data = $port.ReadExisting()
    Write-Host $data
}
catch {
    Write-Host "Error opening COM port: $_"
}
finally {
    $port.Close()
}
