foreach($file in (Get-ChildItem ((Get-Location).Path) -Filter "*.txt")) {
    (Get-Content -Encoding UTF8 $file | ForEach-Object { ([Regex]("([а-яё]+)")).Replace($_, {$args[0].Value.ToUpper()}) } | Set-Content -Encoding UTF8 $file  
}

