export module Inject;
import <string>;
import <Windows.h>;

//Example class
export class Inject
{
public:
	Inject(std::wstring_view dllPath, DWORD ProcessID);

	//The func which injecs the dll
	bool InjectIntoProcess();
private:
	//To init everything
	bool Setup() noexcept;



	HANDLE m_ProcessHandle = nullptr;
	DWORD m_ProcessID = 0;
	std::wstring_view m_DllPath{};
};


