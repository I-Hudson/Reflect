#pragma once

#include <cstdint>

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

		void Message(ConsoleColour textColour, ConsoleColour backgroundColour, const char* fmt, ...);
	}
}

#define Log_Info(fmt, ...) ::Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_White, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);
#define Log_Warn(fmt, ...) ::Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_Yellow, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);
#define Log_Error(fmt, ...) ::Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_Red, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);