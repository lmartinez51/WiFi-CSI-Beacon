$localIP = (Get-NetIPAddress -AddressFamily IPv4 |
    Where-Object {
        $_.IPAddress -notlike "169.*" -and
        $_.IPAddress -ne "127.0.0.1" -and
        $_.PrefixOrigin -ne "WellKnown"
    } |
    Select-Object -First 1 -ExpandProperty IPAddress)

$subnet = ($localIP -split '\.')[0..2] -join '.'

Write-Host "Scanning subnet: $subnet.0/24..."

$results = foreach ($i in 1..254) {
    $ip = "$subnet.$i"

    ping.exe -n 1 -w 300 $ip | Out-Null

    $neighbor = Get-NetNeighbor -IPAddress $ip -ErrorAction SilentlyContinue

    if ($neighbor -and $neighbor.LinkLayerAddress -and $neighbor.State -ne "Unreachable") {
        $hostname = try {
            [System.Net.Dns]::GetHostEntry($ip).HostName
        } catch {
            "Unknown"
        }

        [PSCustomObject]@{
            IPAddress = $ip
            HostName  = $hostname
            MAC       = $neighbor.LinkLayerAddress
            State     = $neighbor.State
        }
    }
}

$results | Sort-Object IPAddress | Format-Table -AutoSize