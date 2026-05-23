$port = New-Object System.IO.Ports.SerialPort COM9, 9600, None, 8, One
$port.Open()
$port.Write("p")
$port.Close()
