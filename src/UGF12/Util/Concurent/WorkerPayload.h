#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>

namespace GxUtil {

	/// <summary>
	/// Templated class to store the access to a concurent worker object for RenderIO
	/// </summary>
	/// <typeparam name="T">Type of worker object</typeparam>
	template<typename T>
	class WorkerPayload {
		public:
			/// <summary>
			/// Clear all flags on construction
			/// </summary>
			WorkerPayload() {
				// Clear all flags
				m_flagCompletion.test_and_set(std::memory_order_acquire);
				m_flagStart.clear(std::memory_order_release);

				// Zero parameter
				ZeroMemory(&m_param, sizeof(T));
			}

			/// <summary>
			/// Signal start condition
			/// </summary>
			void startWork() {
				// Set start flag and clear completion flag
				m_flagCompletion.clear(std::memory_order_release);
				m_flagStart.test_and_set(std::memory_order_acquire);
			}

			/// <summary>
			/// Signal completion
			/// </summary>
			void completeWork() {
				// Clear start condition and set completion
				m_flagStart.clear(std::memory_order_release);
				m_flagCompletion.test_and_set(std::memory_order_acquire);
			}

			/// <summary>
			/// Indicates that a start is required
			/// </summary>
			/// <returns>If start is required</returns>
			BOOL requireStart() {
				return m_flagStart.test(std::memory_order_acquire);
			}

			/// <summary>
			/// Indicates that the action is done
			/// </summary>
			/// <returns>If operation is done</returns>
			BOOL isDone() {
				return m_flagCompletion.test(std::memory_order_acquire);
			}


			/// <summary>
			/// Get the internal parameter
			/// </summary>
			/// <returns>Internal parameter</returns>
			T getParameter() {
				return m_param;
			}

			/// <summary>
			/// Set the internal parameter
			/// </summary>
			/// <param name="param">New value</param>
			void setParameter(T param) {
				m_param = param;
			}

			// Delete unspported
			WorkerPayload(const WorkerPayload&) = delete;
			void operator=(const WorkerPayload&) = delete;
		private:
			/// <summary>
			/// Atomic flag for work start condition
			/// </summary>
			std::atomic_flag m_flagStart;

			/// <summary>
			/// Atomic flag for work completion condition
			/// </summary>
			std::atomic_flag m_flagCompletion;

			/// <summary>
			/// Stored parameter
			/// </summary>
			T m_param;
	};
}