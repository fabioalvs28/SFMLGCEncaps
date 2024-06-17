#pragma once

template<class T>
bool CheckNull(T value)
{
	//check if a pointer is null
	if (value == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<class T, class ...Args>
bool CheckNull(T value, Args ... args)
{
	if (value == nullptr)
	{
		return true;
	}
	else 
	{
		return CheckNull(args...);
	}
}