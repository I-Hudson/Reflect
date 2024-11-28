#pragma once

#include <cstdint>
#include <string>

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

		namespace ConsoleUtils
		{
			void GetConsoleDimensions(uint32_t& width, uint32_t height);	
			uint32_t GetConsoleWidth();
		}

		class ProgressBar
		{
		public:
			enum Options : uint32_t
			{
				None = 0,
				DisplayPercentage = 1 << 0,
				DisplayPostText = 1 << 1,
				DisplayAbsValues = 1 << 2,
				DisplayOnSameLine = 1 << 3,
				 
				DisplayInfo = DisplayPercentage | DisplayPostText | DisplayAbsValues,
				All = ~None
			};
			using OptionsFlags = uint32_t;
			
			ProgressBar();
			ProgressBar(const uint32_t maxProgress, const std::string& text = "", const OptionsFlags options = Options::None);
			ProgressBar(const uint64_t maxProgress, const std::string& text = "", const OptionsFlags options = Options::None);
			~ProgressBar();

			void SetOptions(const OptionsFlags options);
			void SetBarWidth(const uint32_t barWidth);

			void SetText(const std::string& text);
			void SetPostText(const std::string& text);

			void SetPercentage(uint32_t percentage);
			void SetProgress(const uint32_t progress);
			void SetMaxProgress(const uint32_t maxProgress);

			static bool s_ProgressBarLoggedWithoutNewLine;
		private:
			float GetCompletedPercentage() const;
			uint32_t GetBarFillWidth() const;

			uint32_t GetDigitFromNumber(const uint32_t n, const uint32_t idx, uint32_t depth = 0) const;
			uint32_t GetNmberOfDigits(const uint32_t n) const;
			void PrintIntToPrintString(const uint32_t n, uint32_t& printStringIdx);

			uint32_t GetMaxPrintStringLength() const;

			void Print();

		private:
			std::string m_printString;
			std::string m_text;
			std::string m_postText;

			OptionsFlags m_options = Options::None;
			
			uint32_t m_barWidth = 50;
			uint32_t m_currentProgress = 0;
			uint32_t m_maxProgress = 100;
		};
	}
}

#define Log_Info(fmt, ...) ::Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_White, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);
#define Log_Warn(fmt, ...) ::Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_Yellow, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);
#define Log_Error(fmt, ...) ::Reflect::Log::Message(Reflect::Log::ConsoleColour::Light_Red, Reflect::Log::ConsoleColour::Black, fmt, __VA_ARGS__);