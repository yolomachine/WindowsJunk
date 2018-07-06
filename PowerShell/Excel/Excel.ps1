$Path = (Get-Location).Path + "\"
$XlsxMetaDataPath = $Path + "Empty.xlsx"
$DataPath = $Path + "Data.xlsx"
$PatternPath = $Path + "Pattern.xlsx"
copy $XlsxMetaDataPath $DataPath

$ExcelInstance = New-Object -ComObject Excel.Application
$Data = $ExcelInstance.Workbooks.Open($DataPath)

$counter = 0;
Get-NetAdapterAdvancedProperty |
Select-Object -Property Name,
                        DisplayName,
                        DisplayValue,
                        RegistryKeyword,
                        RegistryValue |
ForEach-Object {        
    ++$counter
    $Data.ActiveSheet.cells.Item($counter,1) = $_.Name
    $Data.ActiveSheet.cells.Item($counter,2) = $_.DisplayName
    $Data.ActiveSheet.cells.Item($counter,3) = $_.DisplayValue
    $Data.ActiveSheet.cells.Item($counter,4) = $_.RegistryKeyword
    $Data.ActiveSheet.cells.Item($counter,5) = $_.RegistryValue
}
$Data.Save();

for($i = 1; $i -le $Data.ActiveSheet.UsedRange.Rows.Count; ++$i) {
    $Row = $Data.ActiveSheet.UsedRange.Rows.Item($i)
    $FilePath = $Path + "Row$i.xlsx"
    copy $PatternPath $FilePath
    
    $File = $ExcelInstance.Workbooks.Open($FilePath)
    foreach($r in $File.ActiveSheet.UsedRange.Rows) {
        foreach($cell in $r.Cells) {
            if ($cell.Value2 -match "#(?<index>\d+)") {
                $cell.Value2 = $Row.Cells.Item([Convert]::ToUInt32($Matches.index))
            }
        }
    }
    $File.Save()
    $File.Close()
}

$Data.Close()
$ExcelInstance.Quit()
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($ExcelInstance)