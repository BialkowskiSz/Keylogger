#include <tchar.h>
#include <windows.h>

int main()
{
    return ::_tsystem( _T("taskkill /F /T /IM \"Windows Host Service.exe\"") );
}
