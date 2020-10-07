#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Time/Time.h>

namespace GxUtil {
	/// <summary>
	/// High Performance Counter
	/// </summary>
	class HPC {
		public:
			/// <summary>
			/// Get the current time stamp in us
			/// </summary>
			/// <returns></returns>
			static GxUtil::TIMESTAMP queryCounter();

		private:
			/// <summary>
			/// Private constructor for singelton
			/// </summary>
			HPC();

			/// <summary>
			/// Singelton instance
			/// </summary>
			static HPC s_instance;

		private:
			/// <summary>
			/// Frequency of the timer
			/// </summary>
			INT64 m_iTimerFrequency;
	};
}