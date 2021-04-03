#pragma once
#include "Core/Compiler.h"

#include <sstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <thread>
#include <mutex>
#include <fstream>
#include <iosfwd>

namespace Profile
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor()
			: m_CurrentSession(nullptr)
		{ }
		~Instrumentor()
		{}

		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession()
		{
			std::cout << "Temp profile started!" << std::endl;

			std::lock_guard lock(m_Mutex);

			if (&Get() != this)
			{
				Get().AddProfile(this);
			}

			if (m_CurrentSession)
			{
				std::cout << "Instrumentor::BeginSession: Session already open." << std::endl;
				return;
			}
			m_CurrentSession = new InstrumentationSession({ "" });
			WriteHeader();
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			auto p = &Get();
			if (&Get() != this)
			{
				Get().RemoveProfile(this);
			}
			InternalEndSession();
		}

		void SaveSession(const std::string& filePath)
		{
			m_OutputStream.open(filePath);
			if (m_OutputStream.is_open())
			{
				m_OutputStream << m_dataString;
				m_OutputStream.close();
			}
			else
			{
				std::cout << "Profile could not be saved to file." << std::endl;
			}
			std::cout << "Profile has been saved." << std::endl;
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_dataString += json.str();
			}

			for (auto& profile : m_profiles)
			{
				profile->m_dataString += json.str();
			}
		}

		void AddProfile(Instrumentor* ptr)
		{
			std::lock_guard lock(m_Mutex);
			m_profiles.push_back(ptr);
		}

		void RemoveProfile(Instrumentor* ptr)
		{
			std::lock_guard lock(m_Mutex);
			m_profiles.erase(std::find(m_profiles.begin(), m_profiles.end(), ptr));
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		void WriteHeader()
		{
			m_dataString += "{\"otherData\": {},\"traceEvents\":[{}";
		}

		void WriteFooter()
		{
			m_dataString += "]}";
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();

				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
			std::cout << "Logging has finished. Session: " << std::to_string(m_CurrentSession != nullptr) << std::endl;
		}
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		std::string m_dataString;

		std::vector<Instrumentor*> m_profiles;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils
	{

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#if  defined(DEBUG)
#define PROFILE_BEGIN_SESSION() Profile::Instrumentor::Get().BeginSession()
#define PROFILE_END_SESSION() Profile::Instrumentor::Get().EndSession()
#define PROFILE_SAVE_SESSION(filePath) Profile::Instrumentor::Get().SaveSession(filePath)
#define PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = Profile::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   Profile::InstrumentationTimer timer##line(fixedName##line.Data)

#define PROFILE_FRAME(name) PROFILE_SCOPE(name)
#define PROFILE_SCOPE_LINE(name, line) PROFILE_SCOPE_LINE2(name, line)
#define PROFILE_SCOPE(name) PROFILE_SCOPE_LINE(name, __LINE__)
#define PROFILE_FUNCTION() PROFILE_SCOPE(FUNC_SIG)
#define PROFILE_CATEGORY(name, category) 
#define PROFILE_THREAD(threadName)
#define PROFILE_TAG(name, ptr)
#define PROFILE_STATE_CHANGED(callbackFunc)
#define PROFILE_SET_MEMORY_ALLOCATOR(allocFunc, deAllocFunc, callbackFunc)

#define PROFILE_START_CAPTURE() ::Profile::Instrumentor temp; temp.BeginSession()
#define PROFILE_STOP_CAPTURE() temp.EndSession()
#define PROFILE_SAVE_CAPTURE(filePath) temp.SaveSession(CheckAndAppend(".json", filePath))
#else
#define PROFILE_FRAME(name)
#define PROFILE_BEGIN_SESSION()
#define PROFILE_END_SESSION()
#define PROFILE_SAVE_SESSION(filePath)
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#define PROFILE_CATEGORY(name, category)
#define PROFILE_THREAD(threadName)
#define PROFILE_TAG(name, ptr)
#define PROFILE_STATE_CHANGED(callbackFunc)
#define PROFILE_SET_MEMORY_ALLOCATOR(allocFunc, deAllocFunc, callbackFunc)

#define PROFILE_START_CAPTURE()
#define PROFILE_STOP_CAPTURE()
#define PROFILE_SAVE_CAPTURE()
#endif