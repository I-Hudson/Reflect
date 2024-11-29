#include "Reflect/Core/Log.h"

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
#include <cassert>

namespace Reflect
{
    namespace Log
    {
        void Message(ConsoleColour textColour, ConsoleColour backgroundColour, const char* fmt, ...)
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
				// Small hack. Try and make sure that if any progress bar has been the latest to log and there is no new line.
				// Then insert one here.
				if (ProgressBar::s_ProgressBarLoggedWithoutNewLine)
				{
					std::cout << '\n';
					ProgressBar::s_ProgressBarLoggedWithoutNewLine = false;
				}
				std::cout << buffer;

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

		namespace ConsoleUtils
		{
			void GetConsoleDimensions(uint32_t& width, uint32_t height)
			{
#if defined(_WIN32)
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
				width = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
				height = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__)
				struct winsize w;
				ioctl(fileno(stdout), TIOCGWINSZ, &w);
				width = (int)(w.ws_col);
				height = (int)(w.ws_row);
#endif // Windows/Linux
			}

			uint32_t GetConsoleWidth()
			{
				uint32_t width = 0;
				uint32_t height = 0;
				GetConsoleDimensions(width, height);
				return width;
			}
		}

		bool ProgressBar::s_ProgressBarLoggedWithoutNewLine = false;

		ProgressBar::ProgressBar()
		{ }

		ProgressBar::ProgressBar(const uint32_t maxProgress, const std::string& text, const OptionsFlags options)
			: m_maxProgress(maxProgress)
			, m_text(text)
			, m_options(options)
		{ }

		ProgressBar::ProgressBar(const uint64_t maxProgress, const std::string & text, const OptionsFlags options)
			: m_maxProgress(static_cast<uint32_t>(maxProgress))
			, m_text(text)
			, m_options(options)
		{ }

		ProgressBar::~ProgressBar()
		{ }

		void ProgressBar::SetOptions(const OptionsFlags options)
		{
			m_options = options;
		}

		void ProgressBar::SetBarWidth(const uint32_t barWidth)
		{
			assert(barWidth >= 1);
			m_barWidth = barWidth;
		}

		void ProgressBar::SetText(const std::string& text)
		{
			m_text = text;
			Print();
		}

		void ProgressBar::SetPostText(const std::string& text)
		{
			m_postText = text;
			const uint32_t consoleWidth = ConsoleUtils::GetConsoleWidth();
			const uint32_t printStringLength = GetMaxPrintStringLength();
			if (consoleWidth < printStringLength)
			{
				// Shrink the post text so it fits on one line on the console.
				const uint32_t delta = printStringLength - consoleWidth;
				m_postText.resize(consoleWidth);
			}
			Print();
		}

		void ProgressBar::SetPercentage(uint32_t percentage)
		{
			assert(percentage >= 0 && percentage <= 100);

			float newPercentage = static_cast<float>(percentage);
			float cProgress = static_cast<float>(m_currentProgress);
			float mProgress = static_cast<float>(m_maxProgress);

			const float newProgressDiv = newPercentage / mProgress;
			const float newProgressMul = newProgressDiv * mProgress;

			m_currentProgress = static_cast<uint32_t>(newProgressMul);

			Print();
		}

		void ProgressBar::SetProgress(const uint32_t progress)
		{
			assert(progress >= 0 && progress <= m_maxProgress);

			m_currentProgress = progress;
			Print();
		}

		void ProgressBar::SetMaxProgress(const uint32_t maxProgress)
		{
			m_maxProgress = maxProgress;
		}

		float ProgressBar::GetCompletedPercentage() const
		{
			const float percentage = static_cast<float>(m_currentProgress) / static_cast<float>(m_maxProgress);
			return percentage;
		}

		uint32_t ProgressBar::GetBarFillWidth() const
		{
			const uint32_t barFillWidth = static_cast<uint32_t>(static_cast<float>(m_barWidth) * GetCompletedPercentage());
			return barFillWidth;
		}

		uint32_t ProgressBar::GetDigitFromNumber(const uint32_t n, const uint32_t idx, uint32_t depth) const
		{
			assert(idx < GetNmberOfDigits(n) && depth == 0);

			uint32_t rightMostDigit = n % 10;
			if (idx == depth++)
			{
				return rightMostDigit;
			}

			uint32_t num = n / 10;
			while (num != 0)
			{
				rightMostDigit = num % 10;
				if (idx == depth++)
				{
					return rightMostDigit;
				}

				num = num / 10;
			}

			assert(true);
			return 0;
		}

		uint32_t ProgressBar::GetNmberOfDigits(const uint32_t n) const
		{
			if (n == 0)
			{
				return 1;
			}
			return static_cast<uint32_t>(std::floor(std::log10(n) + 1.0));
		}

		uint32_t ProgressBar::GetMaxPrintStringLength() const
		{
			const uint64_t maxLogSize = strlen("[")
				+ m_barWidth
				+ strlen("]")
				+ strlen(" 100% ")
				+ m_text.size()
				+ strlen(" ")
				+ GetNmberOfDigits(m_maxProgress)
				+ strlen("/")
				+ GetNmberOfDigits(m_maxProgress)
				+ strlen(" ")
				+ m_postText.size()
				+ 1;
			return static_cast<uint32_t>(maxLogSize);
		}

		void ProgressBar::PrintIntToPrintString(const uint32_t n, uint32_t& printStringIdx)
		{
			const uint32_t numberOfDigits = GetNmberOfDigits(n);
			for (uint32_t i = 0; i < numberOfDigits; i++)
			{
				const uint32_t digit = GetDigitFromNumber(n, i);
				const uint32_t idx = printStringIdx + (numberOfDigits - i - 1);

				const char digitChar = '0' + digit;
				m_printString[idx] = digitChar;
			}
			printStringIdx += numberOfDigits;
		}

		void ProgressBar::Print()
		{
			const uint32_t barFillWidth = GetBarFillWidth();

			if (m_printString.size() < GetMaxPrintStringLength())
			{

				m_printString.resize(GetMaxPrintStringLength());
			}

			uint32_t printStringIdx = 0;
			memset(m_printString.data(), ' ', m_printString.size());
			m_printString[printStringIdx++] = '[';

			for (size_t i = 0; i < m_barWidth; i++)
			{
				if (i < barFillWidth)
				{
					m_printString[printStringIdx++] = (char)254u;
				}
				else
				{
					m_printString[printStringIdx++] = '-';
				}
			}
			m_printString[printStringIdx++] = ']';

			if (m_options & Options::DisplayPercentage)
			{
				m_printString[printStringIdx++] = ' ';
				const uint32_t completedPercentage = static_cast<uint32_t>(GetCompletedPercentage() * 100.0f);
				PrintIntToPrintString(completedPercentage, printStringIdx);
				m_printString[printStringIdx++] = '%';
			}

			m_printString[printStringIdx++] = ' ';
			std::copy(m_text.begin(), m_text.end(), m_printString.begin() + printStringIdx);
			printStringIdx += static_cast<uint32_t>(m_text.size());

			if (m_options & Options::DisplayAbsValues)
			{
				m_printString[printStringIdx++] = ' ';
				PrintIntToPrintString(m_currentProgress, printStringIdx);
				m_printString[printStringIdx++] = '/';
				PrintIntToPrintString(m_maxProgress, printStringIdx);
			}
			if (m_options & Options::DisplayPostText)
			{
				m_printString[printStringIdx++] = ' ';
				std::copy(m_postText.begin(), m_postText.end(), m_printString.begin() + printStringIdx);
				printStringIdx += static_cast<uint32_t>(m_postText.size());
			}

			m_printString[printStringIdx++] = '\0';

			if (m_options & Options::DisplayOnSameLine)
			{
				std::cout << /*("\33[2K\r") << */'\r' << m_printString;
				if (m_currentProgress == m_maxProgress)
				{
					std::cout << '\n';
				}
				s_ProgressBarLoggedWithoutNewLine = true;
			}
			else
			{
				std::cout << m_printString << '\n';
				s_ProgressBarLoggedWithoutNewLine = false;
			}
		}
	}
}