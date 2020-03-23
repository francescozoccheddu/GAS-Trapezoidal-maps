#pragma once

#include <string>

namespace GAS
{

	namespace Utils
	{

		/*
			I could have used boost::uuids but it seems that external libraries are not allowed by the project specifications.
			Anyway, this is more plug and play since it already intercepts copy constructions and copy assignments and it produces simpler human-readable serials.
		*/
		class Serial final
		{

			static int s_serial;

			int m_serial { s_serial++ };

			static char encodeDigit (int digit);
			static std::string shortenSerial (int serial);

		public:

			static const std::string null;

			Serial () = default;
			Serial (int serial);
			Serial (const Serial &copy);

			Serial &operator = (const Serial &copy);

			std::string get () const;

			operator std::string () const;

		};

	}

}