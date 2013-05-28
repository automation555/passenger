/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_STR_INT_UTILS_H_
#define _PASSENGER_STR_INT_UTILS_H_

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <ctime>
#include <oxt/macros.hpp>
#include <StaticString.h>

namespace Passenger {

using namespace std;


/**
 * Given a prefix string, a middle string and a postfix string, try to build a string
 * that looks like <tt>prefix + middle + postfix</tt>, with as many characters from
 * <tt>midle</tt> preserved as possible.
 *
 * If <tt>prefix + middle + postfix</tt> does not fit in <tt>max</tt> characters,
 * then <tt>middle</tt> will be truncated so that it fits. If <tt>max</tt> is too
 * small to contain even 1 character from <tt>middle</tt>, then an ArgumentException
 * will be thrown.
 *
 * @code
 *   fillInMiddle(18, "server.", "1234", ".socket");    // "server.1234.socket"
 *   fillInMiddle(16, "server.", "1234", ".socket");    // "server.12.socket"
 *   fillInMiddle(14, "server.", "1234", ".socket");    // ArgumentException
 * @endcode
 *
 * @returns The resulting string, with <tt>middle</tt> possibly truncated.
 * @throws ArgumentException <tt>max</tt> is too small to contain even 1
 *         character from <tt>middle</tt>.
 * @post result.size() <= max
 */
string fillInMiddle(unsigned int max, const string &prefix, const string &middle,
	const string &postfix = "");

/**
 * Checks whether <tt>str</tt> starts with <tt>substr</tt>.
 */
bool startsWith(const StaticString &str, const StaticString &substr);

/**
 * Split the given string using the given separator. Excludes the
 * separator from the output.
 *
 * @param str The string to split.
 * @param sep The separator to use.
 * @param output The vector to write the output to.
 */
void split(const StaticString & restrict_ref str,
	char sep,
	vector<string> & restrict_ref output);
void split(const StaticString & restrict_ref str,
	char sep,
	vector<StaticString> & restrict_ref output);

/**
 * Split the given string using the given separator. Includes the
 * separator in the output, at the end of an item.
 *
 * @param str The string to split.
 * @param sep The separator to use.
 * @param output The vector to write the output to.
 */
void splitIncludeSep(const StaticString & restrict_ref str,
	char sep,
	vector<string> & restrict_ref output);
void splitIncludeSep(const StaticString & restrict_ref str,
	char sep,
	vector<StaticString> & restrict_ref output);

/**
 * Look for 'toFind' inside 'str', replace it with 'replaceWith' and return the result.
 * Only the first occurence of 'toFind' is replaced.
 */
string replaceString(const string &str, const string &toFind, const string &replaceWith);

/**
 * Like replaceString(), but replace all occurrences of `toFind`.
 */
string replaceAll(const string &str, const string &toFind, const string &replaceWith);

/**
 * Strips leading and trailing whitespaces.
 */
string strip(const StaticString &str);

/**
 * Convert anything to a string.
 */
template<typename T> string
toString(T something) {
	stringstream s;
	s << something;
	return s.str();
}

string toString(const vector<string> &vec);
string toString(const vector<StaticString> &vec);

string pointerToIntString(void *pointer);

/**
 * Converts the given integer string to an unsigned long long integer.
 */
unsigned long long stringToULL(const StaticString &str);
unsigned int stringToUint(const StaticString &str);

/**
 * Converts the given integer string to a long long integer.
 */
long long stringToLL(const StaticString &str);
int stringToInt(const StaticString &str);

/**
 * Converts the given hexadecimal string to an unsigned long long integer.
 */
unsigned long long hexToULL(const StaticString &str);
unsigned int hexToUint(const StaticString &str);

/**
 * Converts the given hexatridecimal (base 36) string to an unsigned long long integer.
 */
unsigned long long hexatriToULL(const StaticString &str);

/**
 * Convert the given binary data to hexadecimal.
 */
string toHex(const StaticString &data);

/**
 * Convert the given binary data to hexadecimal. This form accepts an
 * output buffer which must be at least <tt>data.size() * 2</tt> bytes large.
 */
void toHex(const StaticString & restrict_ref data, char * restrict output, bool upperCase = false);

/**
 * Convert the given integer to some other radix, placing
 * the result into the given output buffer. This buffer must be at
 * least <tt>2 * sizeof(IntegerType) + 1</tt> bytes. The output buffer
 * will be NULL terminated. Supported radices are 2-36.
 *
 * @return The size of the created string, excluding
 *         terminating NULL.
 */
template<typename IntegerType, int radix>
unsigned int
integerToOtherBase(IntegerType value, char *output) {
	static const char hex_chars[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y', 'z'
	};
	char buf[sizeof(value) * 2];
	IntegerType remainder = value;
	unsigned int size = 0;
	
	do {
		buf[size] = hex_chars[remainder % radix];
		remainder = remainder / radix;
		size++;
	} while (remainder != 0);
	
	for (unsigned int i = 0; i < size; i++) {
		output[size - i - 1] = buf[i];
	}
	output[size] = '\0';
	return size;
}

/**
 * Convert the given integer to hexadecimal, placing the result
 * into the given output buffer. This buffer must be at least
 * <tt>2 * sizeof(IntegerType) + 1</tt> bytes. The output buffer
 * will be NULL terminated.
 *
 * @return The size of the created hexadecimal string, excluding
 *         terminating NULL.
 */
template<typename IntegerType>
unsigned int
integerToHex(IntegerType value, char *output) {
	return integerToOtherBase<IntegerType, 16>(value, output);
}

/**
 * Convert the given integer to a hexadecimal string.
 */
string integerToHex(long long value);

/**
 * Convert the given integer to hexatridecimal (Base 36), placing the
 * result into the given output buffer. This buffer must be at least
 * <tt>2 * sizeof(IntegerType) + 1</tt> bytes. The output buffer
 * will be NULL terminated.
 *
 * @return The size of the created hexatridecimal string, excluding
 *         terminating NULL.
 */
template<typename IntegerType>
unsigned int
integerToHexatri(IntegerType value, char *output) {
	return integerToOtherBase<IntegerType, 36>(value, output);
}

/**
 * Convert the given integer to a hexatridecimal string.
 */
string integerToHexatri(long long value);

/**
 * Converts the given string to an integer.
 */
int atoi(const string &s);

/**
 * Converts the given string to a long integer.
 */
long atol(const string &s);

/**
 * Round <em>number</em> up to the nearest multiple of <em>multiple</em>.
 */
template<typename IntegerType>
IntegerType
roundUp(IntegerType number, IntegerType multiple) {
	return (number + multiple - 1) / multiple * multiple;
}

/**
 * Compare two strings using a constant time algorithm to avoid timing attacks.
 */
bool constantTimeCompare(const StaticString &a, const StaticString &b);

string distanceOfTimeInWords(time_t fromTime, time_t toTime = 0);

/**
 * Append the given data to the address at 'pos', but do not cross 'end'.
 * Returns the end of the appended string.
 */
char *appendData(char *pos, const char *end, const char *data, size_t size);
char *appendData(char *pos, const char *end, const StaticString &data);

/**
 * Escape non-ASCII-printable characters in the given string with C-style escape sequences,
 * e.g. "foo\nbar\0" becomes "foo\\nbar\\0".
 */
string cEscapeString(const StaticString &input);

/**
 * Escapes HTML special characters the given input string, which is assumed to
 * contain UTF-8 data. Returns a UTF-8 encoded string.
 *
 * @throws utf8::exception A UTF-8 decoding error occurred.
 */
string escapeHTML(const StaticString &input);

StaticString makeStaticStringWithNull(const char *data);

StaticString makeStaticStringWithNull(const string &data);

} // namespace Passenger

#endif /* _PASSENGER_STR_INT_UTILS_H_ */
