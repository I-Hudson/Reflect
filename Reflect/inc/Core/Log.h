#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <iostream>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Reflect
{
	namespace Log
	{
		enum class ConsoleColour : uint8_t
		{
			Black,
			Dark_Blue,
			Draw_Green,
			Draw_Cyan,
			Draw_Red,
			Draw_Magenta,
			Draw_Yellow,
			Draw_White,
			Draw_Grey,

			Light_Blue,
			Light_Green,
			Light_Cyan,
			Light_Red,
			Light_Magenta,
			Light_Yellow,
			Light_White,
		};

		static void Message(ConsoleColour textColour, ConsoleColour backgroundColour, const char* fmt, ...)
		{
			va_list args = {};
			int length = 0;
			char* buffer = nullptr;

			va_start(args, fmt);
			length = _vscprintf(fmt, args) + 1;
			buffer = (char*)malloc(length * sizeof(char));
			if (buffer != nullptr)
			{
				vsprintf_s(buffer, length, fmt, args);
#ifdef _WIN32
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				if (hConsole)
				{
					uint32_t attri = (uint32_t)textColour + ((uint32_t)backgroundColour * 16);
					SetConsoleTextAttribute(hConsole, attri);
				}
				OutputDebugStringA(buffer);
#endif
				std::cout << buffer << '\n';

#ifdef _WIN32
				if (hConsole)
				{
					uint32_t attri = (uint32_t)ConsoleColour::Light_White + ((uint32_t)ConsoleColour::Black * 16);
					SetConsoleTextAttribute(hConsole, attri);
				}
#endif
				free(buffer);
			}
			va_end(args);
		}
	}
}

#define Log_Info(fmt, ...) Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_White, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);
#define Log_Warn(fmt, ...) Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_Yellow, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);
#define Log_Error(fmt, ...) Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_Red, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);