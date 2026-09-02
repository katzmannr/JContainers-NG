#pragma once

#include "common/IErrors.h"
#include <cstdint>

#pragma warning(disable: 4221)
#include <cmath>

typedef std::uint8_t		UInt8;		//!< An unsigned 8-bit integer value
typedef std::uint16_t		UInt16;		//!< An unsigned 16-bit integer value
typedef std::uint32_t		UInt32;		//!< An unsigned 32-bit integer value
typedef std::uint64_t		UInt64;		//!< An unsigned 64-bit integer value
typedef std::int8_t			SInt8;		//!< A signed 8-bit integer value
typedef std::int16_t		SInt16;		//!< A signed 16-bit integer value
typedef std::int32_t		SInt32;		//!< A signed 32-bit integer value
typedef std::int64_t		SInt64;		//!< A signed 64-bit integer value
typedef float				Float32;	//!< A 32-bit floating point value
typedef double				Float64;	//!< A 64-bit floating point value

inline UInt32 Extend16(UInt32 in)
{
	return (in & 0x8000) ? (0xFFFF0000 | in) : in;
}

inline UInt32 Extend8(UInt32 in)
{
	return (in & 0x80) ? (0xFFFFFF00 | in) : in;
}

inline UInt16 Swap16(UInt16 in)
{
	return	((in >> 8) & 0x00FF) |
			((in << 8) & 0xFF00);
}

inline UInt32 Swap32(UInt32 in)
{
	return	((in >> 24) & 0x000000FF) |
			((in >>  8) & 0x0000FF00) |
			((in <<  8) & 0x00FF0000) |
			((in << 24) & 0xFF000000);
}

inline UInt64 Swap64(UInt64 in)
{
	UInt64	temp;

    temp = Swap32(static_cast<UInt32>(in));
    temp <<= static_cast<std::uint32_t>(32);
	temp |= Swap32(in >> 32);

	return temp;
}

inline void SwapFloat(float * in)
{
	UInt32	* temp = (UInt32 *)in;

	*temp = Swap32(*temp);
}

inline void SwapDouble(double * in)
{
	UInt64	* temp = (UInt64 *)in;

	*temp = Swap64(*temp);
}

inline bool IsBigEndian(void)
{
	union
	{
		UInt16	u16;
		UInt8	u8[2];
	} temp;

	temp.u16 = 0x1234;
	
	return temp.u8[0] == 0x12;
}

inline bool IsLittleEndian(void)
{
	return !IsBigEndian();
}

#define CHAR_CODE(a, b, c, d)	(((a & 0xFF) << 0) | ((b & 0xFF) << 8) | ((c & 0xFF) << 16) | ((d & 0xFF) << 24))
#define MACRO_SWAP16(a)			((((a) & 0x00FF) << 8) | (((a) & 0xFF00) >> 8))
#define MACRO_SWAP32(a)			((((a) & 0x000000FF) << 24) | (((a) & 0x0000FF00) << 8) | (((a) & 0x00FF0000) >> 8) | (((a) & 0xFF000000) >> 24))

#define VERSION_CODE(primary, secondary, sub)	(((primary & 0xFFF) << 20) | ((secondary & 0xFFF) << 8) | ((sub & 0xFF) << 0))
#define VERSION_CODE_PRIMARY(in)				((in >> 20) & 0xFFF)
#define VERSION_CODE_SECONDARY(in)				((in >> 8) & 0xFFF)
#define VERSION_CODE_SUB(in)					((in >> 0) & 0xFF)

#define MAKE_COLOR(a, r, g, b)	(((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF) << 0))
#define COLOR_ALPHA(in)			((in >> 24) & 0xFF)
#define COLOR_RED(in)			((in >> 16) & 0xFF)
#define COLOR_GREEN(in)			((in >> 8) & 0xFF)
#define COLOR_BLUE(in)			((in >> 0) & 0xFF)

/**
 *	A 64-bit variable combiner
 *	
 *	Useful for endian-independent value extraction.
 */
union VarCombiner
{
	UInt64	u64;
	SInt64	s64;
	double	f64;
	struct { UInt32 b; UInt32 a; } u32;
	struct { SInt32 b; SInt32 a; } s32;
	struct { float  b; float  a; } f32;
	struct { UInt16 d; UInt16 c; UInt16 b; UInt16 a; } u16;
	struct { SInt16 d; SInt16 c; SInt16 b; SInt16 a; } s16;
	struct { UInt8  h; UInt8  g; UInt8  f; UInt8  e;
			 UInt8  d; UInt8  c; UInt8  b; UInt8  a; } u8;
	struct { SInt8  h; SInt8  g; SInt8  f; SInt8  e;
			 SInt8  d; SInt8  c; SInt8  b; SInt8  a; } s8;
};

/**
 *	A bitfield.
 */
template <typename T>
class Bitfield
{
	public:
				Bitfield()					{ }
				~Bitfield()					{ }
		
		void	Clear(void)					{ field = 0; }						//!< Clears all bits
		void	RawSet(UInt32 data)			{ field = data; }					//!< Modifies all bits
		
		void	Set(UInt32 data)			{ field |= data; }					//!< Sets individual bits
		void	Clear(UInt32 data)			{ field &= ~data; }					//!< Clears individual bits
		void	UnSet(UInt32 data)			{ Clear(data); }					//!< Clears individual bits
		void	Mask(UInt32 data)			{ field &= data; }					//!< Masks individual bits
		void	Toggle(UInt32 data)			{ field ^= data; }					//!< Toggles individual bits
		void	Write(UInt32 data, bool state)
											{ if(state) Set(data); else Clear(data); }
		
		T		Get(void) const				{ return field; }					//!< Gets all bits
		T		Get(UInt32 data) const		{ return field & data; }			//!< Gets individual bits
		T		Extract(UInt32 bit) const	{ return (field >> bit) & 1; }		//!< Extracts a bit
		T		ExtractField(UInt32 shift, UInt32 length)					//!< Extracts a series of bits
											{ return (field >> shift) & (0xFFFFFFFF >> (32 - length)); }
		
		bool	IsSet(UInt32 data) const	{ return ((field & data) == data) ? true : false; }	//!< Are all these bits set?
		bool	IsUnSet(UInt32 data) const	{ return (field & data) ? false : true; }			//!< Are all these bits clear?
		bool	IsClear(UInt32 data) const	{ return IsUnSet(data); }							//!< Are all these bits clear?
	
	private:
		T		field;	//!< bitfield data
};

typedef Bitfield <UInt8>	Bitfield8;		//!< An 8-bit bitfield
typedef Bitfield <UInt16>	Bitfield16;		//!< A 16-bit bitfield
typedef Bitfield <UInt32>	Bitfield32;		//!< A 32-bit bitfield

STATIC_ASSERT(sizeof(Bitfield8) == 1);
STATIC_ASSERT(sizeof(Bitfield16) == 2);
STATIC_ASSERT(sizeof(Bitfield32) == 4);

/**
 *	A bitstring
 *	
 *	Essentially a long bitvector.
 */
class Bitstring
{
	public:
				Bitstring();
				Bitstring(UInt32 inLength);
				~Bitstring();

		void	Alloc(UInt32 inLength);
		void	Dispose(void);

		void	Clear(void);
		void	Clear(UInt32 idx);
		void	Set(UInt32 idx);

		bool	IsSet(UInt32 idx);
		bool	IsClear(UInt32 idx);

	private:
		UInt8	* data;
		UInt32	length;	//!< length in bytes
};

/**
 *	Time storage
 */
class Time
{
	public:
				Time()				{ Clear(); }
				~Time()				{ }
		
		//! Deinitialize the class
		void	Clear(void)			{ seconds = minutes = hours = 0; hasData = false; }
		//! Sets the class to the current time
		//! @todo implement this
		void	SetToNow(void)		{ Set(1, 2, 3); }
		
		//! Sets the class to the specified time
		void	Set(UInt8 inS, UInt8 inM, UInt8 inH)
									{ seconds = inS; minutes = inM; hours = inH; hasData = true; }
		
		//! Gets whether the class has been initialized or not
		bool	IsSet(void)			{ return hasData; }
		
		UInt8	GetSeconds(void)	{ return seconds; }	//!< return the seconds portion of the time
		UInt8	GetMinutes(void)	{ return minutes; }	//!< return the minutes portion of the time
		UInt8	GetHours(void)		{ return hours; }	//!< return the hours portion of the time
	
	private:
		UInt8	seconds, minutes, hours;
		bool	hasData;
};

const float kFloatEpsilon = 0.0001f;

inline bool FloatEqual(float a, float b) { float magnitude = a - b; if(magnitude < 0) magnitude = -magnitude; return magnitude < kFloatEpsilon; }

class Vector2C
{
	public:
		Vector2C() { }
		Vector2C(const Vector2C & in)					{ x = in.x; y = in.y; }
		Vector2C(float inX, float inY)				{ x = inX; y = inY; }
		~Vector2C() { }

		void	Set(float inX, float inY)			{ x = inX; y = inY; }
		void	SetX(float inX)						{ x = inX; }
		void	SetY(float inY)						{ y = inY; }
		void	Get(float * outX, float * outY)		{ *outX = x; *outY = y; }
		float	GetX(void)							{ return x; }
		float	GetY(void)							{ return y; }

		void	Normalize(void)						{ float mag = Magnitude(); x /= mag; y /= mag; }
		float	Magnitude(void)						{ return sqrt(x*x + y*y); }

		void	Reverse(void)						{ float temp = -x; x = -y; y = temp; }

		void	Scale(float scale)					{ x *= scale; y *= scale; }

		void	SwapBytes(void)	{ SwapFloat(&x); SwapFloat(&y); }

		Vector2C &	operator+=(const Vector2C & rhs)	{ x += rhs.x; y += rhs.y; return *this; }
		Vector2C &	operator-=(const Vector2C & rhs)	{ x -= rhs.x; y -= rhs.y; return *this; }
		Vector2C &	operator*=(float rhs)			{ x *= rhs; y *= rhs; return *this; }
		Vector2C &	operator/=(float rhs)			{ x /= rhs; y /= rhs; return *this; }

		float	x;
		float	y;
};

inline Vector2C operator+(const Vector2C & lhs, const Vector2C & rhs)
{
	return Vector2C(lhs.x + rhs.x, lhs.y + rhs.y);
};

inline Vector2C operator-(const Vector2C & lhs, const Vector2C & rhs)
{
	return Vector2C(lhs.x - rhs.x, lhs.y - rhs.y);
};

inline Vector2C operator*(const Vector2C & lhs, float rhs)
{
	return Vector2C(lhs.x * rhs, lhs.y * rhs);
};

inline Vector2C operator/(const Vector2C & lhs, float rhs)
{
	return Vector2C(lhs.x / rhs, lhs.y / rhs);
};

inline bool MaskCompare(void * lhs, void * rhs, void * mask, UInt32 size)
{
	UInt8	* lhs8 = (UInt8 *)lhs;
	UInt8	* rhs8 = (UInt8 *)rhs;
	UInt8	* mask8 = (UInt8 *)mask;

	for(UInt32 i = 0; i < size; i++)
		if((lhs8[i] & mask8[i]) != (rhs8[i] & mask8[i]))
			return false;

	return true;
}

class Vector3C
{
public:
	Vector3C()									{ }
	Vector3C(const Vector3C & in)					{ x = in.x; y = in.y; z = in.z; }
	Vector3C(float inX, float inY, float inZ)	{ x = inX; y = inY; z = inZ; }
	~Vector3C()									{ }

	void	Set(float inX, float inY, float inZ)			{ x = inX; y = inY; z = inZ; }
	void	Get(float * outX, float * outY, float * outZ)	{ *outX = x; *outY = y; *outZ = z; }

	void	Normalize(void)	{ float mag = Magnitude(); x /= mag; y /= mag; z /= mag; }
	float	Magnitude(void)	{ return sqrt(x*x + y*y + z*z); }

	void	Scale(float scale)	{ x *= scale; y *= scale; z *= scale; }

	void	SwapBytes(void)	{ SwapFloat(&x); SwapFloat(&y); SwapFloat(&z); }

	Vector3C &	operator+=(const Vector3C & rhs)	{ x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	Vector3C &	operator-=(const Vector3C & rhs)	{ x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	Vector3C &	operator*=(const Vector3C & rhs)	{ x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
	Vector3C &	operator/=(const Vector3C & rhs)	{ x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

	union
	{
		struct
		{
			float	x, y, z;
		};
		float	d[3];
	};
};

inline Vector3C operator+(const Vector3C & lhs, const Vector3C & rhs)
{
	return Vector3C(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline Vector3C operator-(const Vector3C & lhs, const Vector3C & rhs)
{
	return Vector3C(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline Vector3C operator*(const Vector3C & lhs, const Vector3C & rhs)
{
	return Vector3C(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

inline Vector3C operator/(const Vector3C & lhs, const Vector3C & rhs)
{
	return Vector3C(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
