$port = New-Object System.IO.Ports.SerialPort COM9, 9600, None, 8, One
$port.ReadTimeout = 1000
try {
    $port.Open()
    $port.RtsEnable = $true
    $port.DtrEnable = $true
    Write-Host "--- SMART TESTER ACTIVE ---"
    Write-Host "Step 1: Please press [ON] on your remote."
    $onFound = $false
    $offFound = $false
    $startTime = Get-Date
    
    while ((Get-Date) -lt $startTime.AddMinutes(5)) {
        if ($port.BytesToRead -gt 0) {
            $data = $port.ReadExisting()
            Write-Host $data -NoNewline
            
            if ($data -like "*Captured ON Signal*") { 
                $onFound = $true 
                Write-Host "`r`nStep 2: Please press [OFF] on your remote."
            }
            if ($data -like "*Captured OFF Signal*") { 
                $offFound = $true 
            }
            
            if ($onFound -and $offFound) {
                Write-Host "`r`nBOTH CAPTURED. TRIGGERING AUTOMATIC REPLAY IN 3 SECONDS..."
                Start-Sleep -Seconds 3
                $port.Write("p")
                Write-Host "Command 'p' Sent. Checking Verification..."
                $onFound = $false 
                $offFound = $false
            }
            
            if ($data -like "*[VERIFY]*") {
                Write-Host "`r`n[SUCCESS] TEST COMPLETE. VERIFICATION DETECTED."
                break
            }
        }
        Start-Sleep -Milliseconds 200
    }
}
catch {
    Write-Host "Error: $_"
}
finally {
    $port.Close()
    Write-Host "Port Closed."
}
