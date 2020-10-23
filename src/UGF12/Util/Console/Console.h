#pragma once
#include "pch.h"

#include <UGF12/Util/Exeption.h>
#include <UGF12/Util/Concurent/SpinLock.h>
#include <UGF12/Util/Concurent/LockGuard.h>

#include <imgui.h>

#define UGF12_CONSOLE_MESSAGETYPE_NONE		10
#define UGF12_CONSOLE_MESSAGETYPE_DEBUG		20
#define UGF12_CONSOLE_MESSAGETYPE_INFO		40
#define UGF12_CONSOLE_MESSAGETYPE_WARNING	60
#define UGF12_CONSOLE_MESSAGETYPE_ERROR		80
#define UGF12_CONSOLE_MESSAGETYPE_EXEPTION	100

// Info log (no shipping)
#if defined(SHIPPING)
#define LOG_DEBUG(msg) 
#else
#define LOG_DEBUG(msg) GxUtil::Console::getInstance()->writeString(msg, UGF12_CONSOLE_MESSAGETYPE_DEBUG)
#endif

// Info
#define LOG_INFO(msg) GxUtil::Console::getInstance()->writeString(msg, UGF12_CONSOLE_MESSAGETYPE_INFO)
// Warning
#define LOG_WARNING(msg) GxUtil::Console::getInstance()->writeString(msg, UGF12_CONSOLE_MESSAGETYPE_WARNING)
// Error
#define LOG_ERROR(msg) GxUtil::Console::getInstance()->writeString(msg, UGF12_CONSOLE_MESSAGETYPE_ERROR)
// Exeption
#define LOG_EXEPTION(msg) GxUtil::Console::getInstance()->writeString(msg, UGF12_CONSOLE_MESSAGETYPE_EXEPTION)

namespace GxUtil {
	typedef void (*ConsoleCallback)(LPCSTR, UINT8);

	/// <summary>
	/// Singelton console
	/// </summary>
	class Console {
		public:
			/// <summary>
			/// Get console instance
			/// </summary>
			/// <returns>pointer to console</returns>
			static GxUtil::Console* getInstance();

			/// <summary>
			/// Setup the notify callback
			/// </summary>
			/// <param name="fptrCallback">Pointer to callback</param>
			static void setNotifyCallback(ConsoleCallback fptrCallback);

			/// <summary>
			/// Init the console
			/// </summary>
			/// <param name="bufferSize">size of the buffer in chars</param>
			static void Init(SIZE_T bufferSize, UINT8 messageThreshold);

			/// <summary>
			/// Shutdown the console
			/// </summary>
			static void Shutdown();

			// Delete unsupported
			Console(const Console&) = delete;
			void operator=(const Console&) = delete;

		public:
			/// <summary>
			/// Write a string to the console
			/// </summary>
			/// <param name="strMessage">Message string</param>
			/// <param name="messageType">Message type</param>
			void writeString(LPCSTR strMessage, UINT8 messageType = UGF12_CONSOLE_MESSAGETYPE_INFO);

			/// <summary>
			/// Send a command to the console (only one per frame -> Is safe since im gui will only return one time per frame)
			/// </summary>
			/// <param name="command">Command text</param>
			void sendCommand(LPCSTR command);
			
			/// <summary>
			/// Lock the array and get pointer
			/// </summary>
			/// <param name="ppChar">Pointer to char</param>
			void getReadPointer_lock(CHAR** ppChar);

			/// <summary>
			/// Moves the pointer to the begin of a string
			/// </summary>
			/// <param name="ppChar">Pointer to char pointer (pointer got with getReadPointer_lock)</param>
			/// <param name="ptrMessageType">Pointer to UINT8 witch recives messege type</param>
			/// <returns>If string could be found</returns>
			BOOL getReadPointer_startString(CHAR** ppChar, UINT8* ptrMessageType);

			/// <summary>
			/// Unlock (must be called once after getReadPointer_lock
			/// </summary>
			void getReadPointer_unlock();

			/// <summary>
			/// Increment pointer and assert that posisiton within array is maintained
			/// </summary>
			/// <param name="ppCharToArray">Pointer within m_buffer array</param>
			void pointerIncrement(CHAR** ppCharToArray);

			/// <summary>
			/// Decrement pointer and assert that posisiton within array is maintained
			/// </summary>
			/// <param name="ppCharToArray">Pointer within m_buffer array</param>
			void pointerDecrement(CHAR** ppCharToArray);

			/// <summary>
			/// Get the windows console color for the specified message type
			/// </summary>
			/// <param name="messageType"></param>
			/// <returns></returns>
			DWORD getWindowsColor(UINT8 messageType);

			/// <summary>
			/// Get color for im gui
			/// </summary>
			/// <param name="messageType">Type of message</param>
			/// <returns>Im gui color vector</returns>
			ImVec4 getImGuiColor(UINT8 messageType);

		private:
			/// <summary>
			/// Singelton constructor
			/// </summary>
			/// <param name="bufferSize">Size of the console</param>
			Console(SIZE_T bufferSize, UINT8 messageThreshold);

			// Destructor
			~Console();

			/// <summary>
			/// Singelton instance
			/// </summary>
			static Console* s_ptrInstance;

			/// <summary>
			/// Callback for direct output / sync output (sync: Windows Console; async: ImGui)
			/// </summary>
			static void(*s_fptrNotifyCallback)(LPCSTR, UINT8);

		private:			
			/// <summary>
			/// Writes a char to write head position and increments write head
			/// </summary>
			/// <param name="charToWrite"></param>
			void writeAndIncrmentSafe(CHAR charToWrite);

			/// <summary>
			/// Increment tail until next string
			/// </summary>
			void deleteLastString();
		private:
			/// <summary>
			/// SpinLock for buffer and pointers
			/// </summary>
			GxUtil::SpinLock m_lock;

			/// <summary>
			/// Message threshold
			/// </summary>
			UINT8 m_mesageThreshold;

			/// <summary>
			/// Console buffer
			/// </summary>
			CHAR* m_buffer;

			/// <summary>
			/// Buffer size
			/// </summary>
			SIZE_T m_size;

			/// <summary>
			/// Points to the terminator of the string bevore the first availible string to read
			/// </summary>
			CHAR* m_ptrReadTail;

			/// <summary>
			/// Points to the next char to be written
			/// </summary>
			CHAR* m_ptrWriteHead;
	};
}