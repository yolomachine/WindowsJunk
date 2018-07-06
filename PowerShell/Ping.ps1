Param ([string]$Domain, [string]$Mask)

function IP-to-Int64 ([string]$ip) {
  $octets = $ip.split(".") 
  return [int64]([int64]$octets[0]*16777216 +[int64]$octets[1]*65536 +[int64]$octets[2]*256 +[int64]$octets[3]) 
} 
 
function Int64-to-IP([int64]$int) {
  $octets = ([math]::truncate($int/16777216)).tostring()+"."+([math]::truncate(($int%16777216)/65536)).tostring()+"."+([math]::truncate(($int%65536)/256)).tostring()+"."+([math]::truncate($int%256)).tostring() 
  return $octets
} 

$DomainDescriptor = (Test-Connection -Count 1 $Domain).IPV4Address | Select-Object -first 1
echo $DomainDescriptor

$Bits = 0
(($Mask -split '\.' | % { [convert]::ToString($_,2) } ) -join '').tochararray() | % { $Bits += ([convert]::ToInt32($_) - 48)}
$Range = [Math]::Pow(2, 32 - $Bits)
$Network = Int64-to-IP((IP-to-Int64 $DomainDescriptor.IPAddressToString) -band (IP-to-Int64 $Mask))
for ($i = 0; $i -lt $Range; ++$i) {
    $ip = (Int64-to-IP ((IP-to-Int64 $network) + $i))
    if (Test-Connection -Quiet -Count 1 $ip) {
        echo "$ip success"
    } else {
        echo "$ip failure"
    }
}