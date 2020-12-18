param ($file, $value, $offset)

$bytes = [System.IO.File]::ReadAllBytes($file);
$numar  = [System.BitConverter]::GetBytes([Int]$value);
$bytes[[Int]$offset + 0] = $numar[0];
$bytes[[Int]$offset + 1] = $numar[1];
$bytes[[Int]$offset + 2] = $numar[2];
$bytes[[Int]$offset + 3] = $numar[3];
[System.IO.File]::WriteAllBytes($file, $bytes);