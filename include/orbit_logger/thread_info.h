/*
  * Generated by cppsrc.sh
  * On 2015-03-29 12:49:26,87
  * by Paweu
*/
/*--END OF HEADER BLOCK--*/

#pragma once
#ifndef ThreadInfo_H
#define ThreadInfo_H

namespace OrbitLogger {

class ThreadInfo {
public:
	using Signature = uint32_t;
	using NumericID = uint32_t;
	struct SignatureBuffer {
		char m_String[sizeof(Signature) + 1];

		SignatureBuffer(Signature s) {
			*((Signature*)m_String) = s;
			m_String[sizeof(Signature)] = 0;
		}
	};

	static bool IsMain() { return _IsMain; }
	static Signature GetSignature() { return _ThisThreadSignature; }
	static NumericID GetID();
	static void SetName(const char* sign, bool IsMain = false);
protected:
	thread_local static bool _IsMain;
	thread_local static Signature _ThisThreadSignature;
};

} //namespace OrbitLogger

#endif
