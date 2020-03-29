/// GAS::Utils::Serial class for debugging purposes.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_UTILS_SERIAL_INCLUDED
#define GAS_UTILS_SERIAL_INCLUDED

#include <string>

namespace GAS
{

	namespace Utils
	{

		/// Small class that provides unique serials for object tagging.
		/// \note
		/// I could have used \c boost::uuids but it seems that external libraries are not allowed by the project specifications.
		/// Anyway, this is more plug and play since it already intercepts copy constructions and copy assignments and it produces simpler human-readable serials.
		class Serial final
		{

			/// Auto incrementing global counter.
			static int s_serial;

			int m_serial { s_serial++ };

			/// Encode a base 36 digit into an easy-to-read alphanumeric character.
			/// \param[in] digit
			/// The digit to encode.
			/// \pre
			/// \p digit must fall in range [0,35].
			/// \return
			/// The encoded character.
			static char encodeDigit (int digit);

			/// Encode a numeric serial into a shorter easy-to-read alphanumeric string.
			/// \param[in] serial
			/// The serial to encode.
			/// \return
			/// The encoded string.
			static std::string shortenSerial (int serial);

		public:

			/// Out of domain serial for representing null objects.
			static const std::string null;

			/// Construct a serial tag using the auto incrementing global counter.
			Serial () = default;

			/// Construct a serial tag using the specified serial.
			/// \param[in] serial
			/// The serial.
			Serial (int serial);

			/// Construct a serial tag using the auto incrementing global counter.
			/// \param[in] copy
			/// This parameter is purposely ignored.
			/// \remark
			/// This constructor ignores the \p copy parameter in order to avoid requiring the tagged object
			/// to provide custom logic for preventing serial duplication on copy.
			Serial (const Serial &copy);

			/// Does nothing.
			/// \param[in] copy
			/// This parameter is purposely ignored.
			/// \remark
			/// This assignment operator ignores the \p copy parameter in order to avoid requiring the tagged object
			/// to provide custom logic for preventing serial duplication on assignment.
			Serial &operator = (const Serial &copy);

			/// Get the human-readable serial.
			/// \return
			/// The human-readable serial.
			std::string get () const;

			/// \copydoc get
			operator std::string () const;

		};

	}

}

#endif