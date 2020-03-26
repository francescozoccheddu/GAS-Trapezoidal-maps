#pragma once

#include "serial.hpp"

#include <cassert>
#include <cmath>

namespace GAS
{

	namespace Utils
	{

		int Serial::s_serial {};

		char Serial::encodeDigit (int _digit)
		{
			assert (_digit >= 0 && _digit < 10 + 26);
			const char offset { static_cast<char>(_digit) };
			if (_digit < 10)
			{
				return _digit + '0';
			}
			else
			{
				return _digit - 10 + 'A';
			}
		}

		std::string Serial::shortenSerial (int _serial)
		{
			constexpr int digitCount { 10 + 26 };
			std::string result;
			int remainder { std::abs (_serial) };
			while (remainder >= digitCount)
			{
				result.insert (0, 1, encodeDigit (remainder % digitCount));
				remainder /= digitCount;
			}
			result.insert (0, 1, encodeDigit (remainder));
			return result;
		}

		const std::string Serial::null { '-' };

		Serial::Serial (int _serial) : m_serial { _serial }
		{}

		Serial::Serial (const Serial &/*_copy*/)
		{
			// Intercept and prevent serial copy
		}

		Serial &Serial::operator=(const Serial &/*_copy*/)
		{
			// Intercept and prevent serial assignment
			return *this;
		}

		std::string Serial::get () const
		{
			return shortenSerial (m_serial);
		}

		Serial::operator std::string () const
		{
			return get ();
		}

	}

}