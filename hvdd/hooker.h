typedef int (WINAPI *pMessageBox)(HWND, LPCWSTR, LPCWSTR, UINT);
typedef int (WINAPI *pMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);
typedef HMODULE (WINAPI *pLoadLibraryW)(LPCWSTR);
typedef FARPROC (WINAPI *pGetProcAddress)(HMODULE, LPCSTR);
typedef HANDLE (WINAPI *pCreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef BOOL (WINAPI *pVidReadMemoryBlockPageRange)(HANDLE, PVOID, UINT64, UINT64, PVOID, UINT64);
typedef BOOL (WINAPI *pVidWriteMemoryBlockPageRange)(HANDLE, PVOID, UINT64, UINT64, PVOID, UINT64);
typedef DWORD (WINAPI *pSetFilePointer)(HANDLE, LONG, PLONG, DWORD);
typedef LPVOID (WINAPI *pVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);
typedef BOOL (WINAPI *pVirtualFree)(LPVOID, SIZE_T, DWORD);
typedef BOOL (WINAPI *pVirtualProtect)(LPVOID, SIZE_T, DWORD, PDWORD);
typedef HANDLE (WINAPI *pCreateFileMappingA)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCSTR);
typedef HANDLE (WINAPI *pCreateFileMappingW)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCWSTR);
typedef LPVOID (WINAPI *pMapViewOfFile)(HANDLE, DWORD, DWORD, DWORD, SIZE_T);
typedef BOOL (WINAPI *pUnmapViewOfFile)(LPCVOID);
typedef DWORD (WINAPI *pGetFileSize)(HANDLE, LPDWORD);
typedef BOOL (WINAPI *pReadFile)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);

typedef struct _MAP_FILE {
    PVOID Va;
    PHYSICAL_ADDRESS Pa;
    ULONG Size;
} MAP_FILE, *PMAP_FILE;

typedef struct _FUNCTION_TABLE {
    pLoadLibraryW _LoadLibrary;
    pGetProcAddress _GetProcAddress;
    pMessageBox _MessageBoxW;
    pMessageBoxA _MessageBoxA;
    pCreateFileW _CreateFileW;
    pSetFilePointer _SetFilePointer;
    pVirtualAlloc _VirtualAlloc;
    pVirtualFree _VirtualFree;
    pVirtualProtect _VirtualProtect;
    pVidReadMemoryBlockPageRange _VidReadMemoryBlockPageRange;
    pVidWriteMemoryBlockPageRange _VidWriteMemoryBlockPageRange;
    pCreateFileMappingA _CreateFileMappingA;
    pCreateFileMappingW _CreateFileMappingW;
    pMapViewOfFile _MapViewOfFile;
    pUnmapViewOfFile _UnmapViewOfFile;
    pGetFileSize _GetFileSize;
    pReadFile _ReadFile;
    LARGE_INTEGER FileSize;
    HANDLE CrashDumpHandle;
    PUCHAR Header;
    ULONG HeaderSize;
    ULONG ContextPageIndex;
    ULONG ContextOffsetLow;
    HANDLE PartitionHandle;
    MB_HANDLE MemoryHandle;
    MACHINE_TYPE MachineType;
	//MAP_FILE MapFile[30];
    MAP_FILE MapFile[20];
    ULONG MapIndex;
} FUNCTION_TABLE, *PFUNCTION_TABLE;



//#define TABLE_OFFSET (PVOID)(NULL)
#define TABLE_OFFSET (PVOID)(0xFFF000000)
#define CREATEFILE_OFFSET (PVOID)((PUCHAR)TABLE_OFFSET + sizeof(FUNCTION_TABLE))
#define CREATEFILEMAPPINGA_OFFSET (PVOID)((PUCHAR)CREATEFILE_OFFSET + 0x300)
#define CREATEFILEMAPPINGW_OFFSET (PVOID)((PUCHAR)CREATEFILEMAPPINGA_OFFSET + 0x100)
#define MAPVIEWOFFILE_OFFSET (PVOID)((PUCHAR)CREATEFILEMAPPINGW_OFFSET + 0x100)
#define UNMAPVIEWOFFILE_OFFSET (PVOID)((PUCHAR)MAPVIEWOFFILE_OFFSET + 0xA00)
#define GETFILESIZE_OFFSET (PVOID)((PUCHAR)UNMAPVIEWOFFILE_OFFSET + 0x100)
#define READFILE_OFFSET (PVOID)((PUCHAR)GETFILESIZE_OFFSET + 0x100)
#define VIRTUALPROTECT_OFFSET (PVOID)((PUCHAR)READFILE_OFFSET + 0x100)

//#define CREATEFILE_OFFSET 0x2b0
//#define CREATEFILEMAPPINGA_OFFSET CREATEFILE_OFFSET + 0x300
//#define CREATEFILEMAPPINGW_OFFSET CREATEFILEMAPPINGA_OFFSET + 0x100
//#define MAPVIEWOFFILE_OFFSET CREATEFILEMAPPINGW_OFFSET + 0x100
//#define UNMAPVIEWOFFILE_OFFSET MAPVIEWOFFILE_OFFSET + 0x400
//#define GETFILESIZE_OFFSET UNMAPVIEWOFFILE_OFFSET + 0x100
//#define READFILE_OFFSET GETFILESIZE_OFFSET + 0x100
//#define VIRTUALPROTECT_OFFSET READFILE_OFFSET + 0x100

#define VM_PAGE_COUNT (1024 * 1024 * 1024 * 2 / 0x1000)


extern FUNCTION_TABLE FunctionTable;

HANDLE WINAPI MyCreateFile(
  __in      LPCWSTR lpFileName,
  __in      DWORD dwDesiredAccess,
  __in      DWORD dwShareMode,
  __in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  __in      DWORD dwCreationDisposition,
  __in      DWORD dwFlagsAndAttributes,
  __in_opt  HANDLE hTemplateFile
);

LPVOID WINAPI MyMapViewOfFile(
  __in  HANDLE hFileMappingObject,
  __in  DWORD dwDesiredAccess,
  __in  DWORD dwFileOffsetHigh,
  __in  DWORD dwFileOffsetLow,
  __in  SIZE_T dwNumberOfBytesToMap
);

HANDLE WINAPI MyCreateFileMappingA(
  __in      HANDLE hFile,
  __in_opt  LPSECURITY_ATTRIBUTES lpAttributes,
  __in      DWORD flProtect,
  __in      DWORD dwMaximumSizeHigh,
  __in      DWORD dwMaximumSizeLow,
  __in_opt  LPCSTR lpName
);

HANDLE WINAPI MyCreateFileMappingW(
  __in      HANDLE hFile,
  __in_opt  LPSECURITY_ATTRIBUTES lpAttributes,
  __in      DWORD flProtect,
  __in      DWORD dwMaximumSizeHigh,
  __in      DWORD dwMaximumSizeLow,
  __in_opt  LPCWSTR lpName
);

BOOL WINAPI MyUnmapViewOfFile(
  __in  LPCVOID lpBaseAddress
);

DWORD WINAPI MyGetFileSize(
  __in       HANDLE hFile,
  __out_opt  LPDWORD lpFileSizeHigh
);

BOOL WINAPI MyReadFile(
  __in         HANDLE hFile,
  __out        LPVOID lpBuffer,
  __in         DWORD nNumberOfBytesToRead,
  __out_opt    LPDWORD lpNumberOfBytesRead,
  __inout_opt  LPOVERLAPPED lpOverlapped
);

BOOL WINAPI MyVirtualProtect(
  __in   LPVOID lpAddress,
  __in   SIZE_T dwSize,
  __in   DWORD flNewProtect,
  __out  PDWORD lpflOldProtect
);

BOOL
HookKd(
    HANDLE ProcessHandle,
    ULONG ProcessId
);