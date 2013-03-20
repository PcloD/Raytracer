/**
 * Copyright (c) 2006 Manuel Bua
 *
 * THIS SOFTWARE IS PROVIDED 'AS-IS', WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY. IN NO EVENT WILL THE AUTHORS BE HELD LIABLE FOR ANY DAMAGES
 * ARISING FROM THE USE OF THIS SOFTWARE.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not
 *     be misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */

#ifndef AQUA_FIXEDPOINT_H
#define AQUA_FIXEDPOINT_H

#define bool_t bool
#define int32_t int
#define uint32_t unsigned int
#define double_t double
#define float_t float
#define int64_t __int64

namespace Aqua
{

	/**
	 *	Mantains precalculated basic information such
	 *	as bit-masks and other constants.
	 */
	template<int32_t precision_bits>
	class AquaFixedInfo
	{
		protected:
			enum { FRACTION_MASK	= ( 1 << precision_bits ) - 1		};
			enum { ONE				= ( 1 << precision_bits )			};
			enum { ROUND			= ( 1 << ( precision_bits - 1 ) )	};
			enum { HALF_BITS		= ( precision_bits / 2 )			};
			enum { SIGN_BIT			= ( 1 << 31 )						};
	};


	/**
	 *	More comments for me ...
	 */
	template<int32_t precision_bits,
			 template <int32_t> class MulPrecisionPolicy,
			 template <int32_t> class DivPrecisionPolicy>
	class AquaFixed : public AquaFixedInfo<precision_bits>
	{
		public:


			/**
			 *	Construction w/ precision conversion
			 */
			template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
			explicit AquaFixed( const AquaFixed<bits, mulP, divP>& rhs )
			{
				v = translate( rhs.v, bits );
			}


			/**
			 *	Assignment w/ precision conversion
			 *
			 *	\think
			 *		remove because subtle ambiguities?
			 */
			template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
			inline AquaFixed& operator=( const AquaFixed<bits, mulP, divP>& rhs )
			{
				v = translate( rhs.v, bits );
				return *this;
			}


			/** Construction */
			inline static AquaFixed fromRaw( int32_t raw )	{ AquaFixed tmp; tmp.v = raw; return tmp; }
			AquaFixed() : v( 0 ) {}
			AquaFixed( const AquaFixed& rhs )	: v( rhs.v ) {}
			explicit AquaFixed( float_t rhs )			: v( (int32_t)( rhs *  (float_t)ONE + ( rhs < 0 ? -0.5f : 0.5f ) ) ) {}
			explicit AquaFixed( double_t rhs )			: v( (int32_t)( rhs * (double_t)ONE + ( rhs < 0 ? -0.5f : 0.5f ) ) ) {}
			explicit AquaFixed( int32_t rhs )			: v( rhs << precision_bits ) {}

			/** AquaFixed assignment */
			inline AquaFixed& operator=( const AquaFixed& rhs )		{ v = rhs.v; return *this; }
			inline AquaFixed& operator+=( const AquaFixed& rhs )	{ v += rhs.v; return *this; }
			inline AquaFixed& operator-=( const AquaFixed& rhs )	{ v -= rhs.v; return *this; }
			//inline AquaFixed& operator*=( const AquaFixed& rhs )	{ v = mul_t::mul( v, rhs.v ); return *this; }
			//inline AquaFixed& operator/=( const AquaFixed& rhs )	{ v = div_t::div( v, rhs.v ); return *this; }
			inline AquaFixed& operator*=( const AquaFixed& rhs )	{ v = MulPrecisionPolicy<precision_bits>::mul( v, rhs.v ); return *this; }
			inline AquaFixed& operator/=( const AquaFixed& rhs )	{ v = DivPrecisionPolicy<precision_bits>::div( v, rhs.v ); return *this; }

			/** int32_t assignment */
			inline AquaFixed& operator=( int32_t rhs )				{ v = AquaFixed( rhs ).v; return *this; }
			inline AquaFixed& operator+=( int32_t rhs )				{ v += rhs << precision_bits; return *this; }
			inline AquaFixed& operator-=( int32_t rhs )				{ v -= rhs << precision_bits; return *this; }
			inline AquaFixed& operator*=( int32_t rhs )				{ v *= rhs; return *this; }
			inline AquaFixed& operator/=( int32_t rhs )				{ v /= rhs; return *this; }

			/** float_t assignment */
			inline AquaFixed& operator=( float_t rhs )				{ v = AquaFixed( rhs ).v; return *this; }
			inline AquaFixed& operator+=( float_t rhs )				{ v += AquaFixed( rhs ).v; return *this; }
			inline AquaFixed& operator-=( float_t rhs )				{ v -= AquaFixed( rhs ).v; return *this; }
			inline AquaFixed& operator*=( float_t rhs )				{ *this *= AquaFixed( rhs ); return *this; }
			inline AquaFixed& operator/=( float_t rhs )				{ *this /= AquaFixed( rhs ); return *this; }

			/** double_t assignment */
			inline AquaFixed& operator=( double_t rhs )				{ v = AquaFixed( rhs ).v; return *this; }
			inline AquaFixed& operator+=( double_t rhs )			{ v += AquaFixed( rhs ).v; return *this; }
			inline AquaFixed& operator-=( double_t rhs )			{ v -= AquaFixed( rhs ).v; return *this; }
			inline AquaFixed& operator*=( double_t rhs )			{ *this *= AquaFixed( rhs ); return *this; }
			inline AquaFixed& operator/=( double_t rhs )			{ *this /= AquaFixed( rhs ); return *this; }

			/** shift assignment */
			inline AquaFixed& operator<<=( int32_t shift )			{ v <<= shift; return *this; }
			inline AquaFixed& operator>>=( int32_t shift )			{ v >>= shift; return *this; }

			/** converters */
			inline operator int32_t()								{ return v >> precision_bits; }
			inline operator float_t()								{ return v * ( 1.0f / static_cast<float_t>( ONE ) ); }
			inline operator double_t()								{ return v * ( 1.0f / static_cast<double_t>( ONE ) ); }


			/**
			 *	Gives access to the integer part of the number.
			 *
			 *	An use of the integer portion could be for lerping fixed-point
			 *	quantities.
			 */
			inline int32_t		getInteger() const  { return( v >> precision_bits ); }

			/**
			 *	Gives access to the fractional part of the number.
			 */
			inline uint32_t		getFraction() const { return( v & FRACTION_MASK ); }

			inline int32_t		getRaw() const		{ return v; }

			/** Unary operators */
			inline AquaFixed operator+() const		{ return AquaFixed::fromRaw(  v ); }
			inline AquaFixed operator-() const		{ return AquaFixed::fromRaw( -v ); }


			/** Comparison operators */

			// AquaFixed
			inline bool_t operator==( const AquaFixed& other )		{ return v == other.v; }
			inline bool_t operator!=( const AquaFixed& other )		{ return v != other.v; }
			inline bool_t operator<( const AquaFixed& other )		{ return v  < other.v; }
			inline bool_t operator>( const AquaFixed& other )		{ return v  > other.v; }
			inline bool_t operator<=( const AquaFixed& other )		{ return v <= other.v; }
			inline bool_t operator>=( const AquaFixed& other )		{ return v >= other.v; }

			// int32_t
			inline bool_t operator==( int32_t other )				{ return v == AquaFixed( other ).v; }
			inline bool_t operator!=( int32_t other )				{ return v != AquaFixed( other ).v; }
			inline bool_t operator<( int32_t other )				{ return v  < AquaFixed( other ).v; }
			inline bool_t operator>( int32_t other )				{ return v  > AquaFixed( other ).v; }
			inline bool_t operator<=( int32_t other )				{ return v <= AquaFixed( other ).v; }
			inline bool_t operator>=( int32_t other )				{ return v >= AquaFixed( other ).v; }

			// float
			inline bool_t operator==( float_t other )				{ return v == AquaFixed( other ).v; }
			inline bool_t operator!=( float_t other )				{ return v != AquaFixed( other ).v; }
			inline bool_t operator<( float_t other )				{ return v  < AquaFixed( other ).v; }
			inline bool_t operator>( float_t other )				{ return v  > AquaFixed( other ).v; }
			inline bool_t operator<=( float_t other )				{ return v <= AquaFixed( other ).v; }
			inline bool_t operator>=( float_t other )				{ return v >= AquaFixed( other ).v; }

			// double
			inline bool_t operator==( double_t other )				{ return v == AquaFixed( other ).v; }
			inline bool_t operator!=( double_t other )				{ return v != AquaFixed( other ).v; }
			inline bool_t operator<( double_t other )				{ return v  < AquaFixed( other ).v; }
			inline bool_t operator>( double_t other )				{ return v  > AquaFixed( other ).v; }
			inline bool_t operator<=( double_t other )				{ return v <= AquaFixed( other ).v; }
			inline bool_t operator>=( double_t other )				{ return v >= AquaFixed( other ).v; }


			/** Utilities */
/*
			inline AquaFixed mulDiv( const AquaFixed& m1, const AquaFixed& m2, const AquaFixed& d )
			{
				int64_t t = int64_t( m1 ) * int64_t( m2 );
				t += static_cast<int32_t>( ROUND );
				t /= d;
				return int32_t(t);
			}
*/
			inline AquaFixed abs()
			{
				return AquaFixed::fromRaw( ( v & ( 1 << 31 ) ) ? -v : v );
			}
			
			inline AquaFixed sign()
			{
				return AquaFixed::fromRaw( ( v & ( 1 << 31 ) ) ? -static_cast<int32_t>( ONE ): static_cast<int32_t>( ONE ) );
			}
			
			inline AquaFixed floor()
			{
				return AquaFixed( (int32_t)( v >> precision_bits ) );
			}
			
			inline AquaFixed ceil()
			{
				int32_t tmp = -( -v >> precision_bits );
				return AquaFixed( tmp );
			}
			
			inline AquaFixed round()
			{
				int64_t tmp = int64_t( v );
				if( tmp > 0 ) tmp += static_cast<int32_t>( ROUND ); else tmp-= static_cast<int32_t>( ROUND );
				tmp /= static_cast<int32_t>( ONE );
				return AquaFixed( int32_t( tmp ) );
			}


		private:

			int32_t v;

			inline static int32_t translate( int32_t src, int32_t bits )
			{
				if( bits < precision_bits )
				{
					return( src << ( precision_bits - bits ) );
				}
				else
				{
					return( src >> ( bits - precision_bits ) );
				}
			}


			//friend AquaFixed<0>;    friend AquaFixed<1>;    friend AquaFixed<2>;    friend AquaFixed<3>;
			//friend AquaFixed<4>;    friend AquaFixed<5>;    friend AquaFixed<6>;    friend AquaFixed<7>;
			//friend AquaFixed<8>;    friend AquaFixed<9>;    friend AquaFixed<10>;   friend AquaFixed<11>;
			//friend AquaFixed<12>;   friend AquaFixed<13>;   friend AquaFixed<14>;   friend AquaFixed<15>;
			//friend AquaFixed<16>;   friend AquaFixed<17>;   friend AquaFixed<18>;   friend AquaFixed<19>;
			//friend AquaFixed<20>;   friend AquaFixed<21>;   friend AquaFixed<22>;   friend AquaFixed<23>;
			//friend AquaFixed<24>;   friend AquaFixed<25>;   friend AquaFixed<26>;   friend AquaFixed<27>;
			//friend AquaFixed<28>;   friend AquaFixed<29>;   friend AquaFixed<30>;   friend AquaFixed<31>;
	};


	//////////////////////////////////////////////////////////////////////////
	// "Add" operation on AquaFixed (global)
	//////////////////////////////////////////////////////////////////////////


	// op: AquaFixed + AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator+( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result += rhs;
		return result;
	}


	/**
	 *	int32_t
	 */

	// op: AquaFixed + int32_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator+( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result += rhs;
		return result;
	}

	// op: int32_t + AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator+( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return rhs + lhs;
	}


	/**
	 *	float_t
	 */

	// op: AquaFixed + float_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator+( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result += rhs;
		return result;
	}

	// op: float_t + AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator+( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return rhs + lhs;
	}


	/**
	 *	double_t
	 */

	// op: AquaFixed + double_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator+( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result += rhs;
		return result;
	}

	// op: double_t + AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator+( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return rhs + lhs;
	}


	//////////////////////////////////////////////////////////////////////////
	// "Subtract" operation on AquaFixed (global)
	//////////////////////////////////////////////////////////////////////////


	// op: AquaFixed - AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator-( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result -= rhs;
		return result;
	}


	/**
	 *	int32_t
	 */

	// op: AquaFixed - int32_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator-( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result -= rhs;
		return result;
	}

	// op: int32_t - AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator-( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return AquaFixed<bits, mulP, divP>( lhs ) - rhs;
	}


	/**
	 *	float_t
	 */

	// op: AquaFixed - float_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator-( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result -= rhs;
		return result;
	}

	// op: float_t - AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator-( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return AquaFixed<bits, mulP, divP>( lhs ) - rhs;
	}


	/**
	 *	double_t
	 */

	// op: AquaFixed - double_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator-( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result -= rhs;
		return result;
	}

	// op: double_t - AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator-( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return AquaFixed<bits, mulP, divP>( lhs ) - rhs;
	}


	//////////////////////////////////////////////////////////////////////////
	// "Multiply" operation on AquaFixed (global)
	//////////////////////////////////////////////////////////////////////////


	// op: AquaFixed * AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator*( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result *= rhs;
		return result;
	}


	/**
	*	int32_t
	*/

	// op: AquaFixed * int32_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator*( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result *= rhs;
		return result;
	}

	// op: int32_t * AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator*( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return rhs * lhs;
	}


	/**
	*	float_t
	*/

	// op: AquaFixed * float_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator*( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result *= rhs;
		return result;
	}

	// op: float_t * AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator*( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return rhs * lhs;
	}


	/**
	*	double_t
	*/

	// op: AquaFixed * double_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator*( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result *= rhs;
		return result;
	}

	// op: double_t * AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator*( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return rhs * lhs;
	}


	//////////////////////////////////////////////////////////////////////////
	// "Division" operation on AquaFixed (global)
	//////////////////////////////////////////////////////////////////////////


	// op: AquaFixed / AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator/( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result /= rhs;
		return result;
	}


	/**
	 *	int32_t
	 */

	// op: AquaFixed / int32_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator/( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result /= rhs;
		return result;
	}

	// op: int32_t / AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator/( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return AquaFixed<bits, mulP, divP>( lhs ) / rhs;
	}


	/**
	 *	float_t
	 */

	// op: AquaFixed / float_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator/( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result /= rhs;
		return result;
	}

	// op: float_t / AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator/( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return AquaFixed<bits, mulP, divP>( lhs ) / rhs;
	}


	/**
	 *	double_t
	 */

	// op: AquaFixed / double_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator/( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )
	{
		AquaFixed<bits, mulP, divP> result( lhs );
		result /= rhs;
		return result;
	}

	// op: double_t / AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator/( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )
	{
		return AquaFixed<bits, mulP, divP>( lhs ) / rhs;
	}


	//////////////////////////////////////////////////////////////////////////
	// Shift operators
	//////////////////////////////////////////////////////////////////////////

	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator<<( const AquaFixed<bits, mulP, divP>& x, int32_t shift )
	{
		AquaFixed<bits, mulP, divP> result( x );
		result <<= shift;
		return result;
	}

	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>
	inline AquaFixed<bits, mulP, divP> operator>>( const AquaFixed<bits, mulP, divP>& x, int32_t shift )
	{
		AquaFixed<bits, mulP, divP> result( x );
		result >>= shift;
		return result;
	}


	//////////////////////////////////////////////////////////////////////////
	// Comparison operators
	//////////////////////////////////////////////////////////////////////////

	// AquaFixed
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator==( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )	{ return lhs == rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator!=( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )	{ return lhs != rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )	{ return lhs  < rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<=( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )	{ return lhs <= rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )	{ return lhs  > rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>=( const AquaFixed<bits, mulP, divP>& lhs, const AquaFixed<bits, mulP, divP>& rhs )	{ return lhs >= rhs;	}

	// int32_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator==( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs ) == rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator!=( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs ) != rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs )  < rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<=( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs ) <= rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs )  > rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>=( int32_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs ) >= rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator==( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )					{ return lhs == AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator!=( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )					{ return lhs != AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )					{ return lhs <  AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<=( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )					{ return lhs <= AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )					{ return lhs >  AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>=( const AquaFixed<bits, mulP, divP>& lhs, int32_t rhs )					{ return lhs >= AquaFixed<bits, mulP, divP>( rhs );	}

	// float_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator==( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs ) == rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator!=( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs ) != rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs )  < rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<=( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs ) <= rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs )  > rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>=( float_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs ) >= rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator==( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )					{ return lhs == AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator!=( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )					{ return lhs != AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )					{ return lhs <  AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<=( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )					{ return lhs <= AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )					{ return lhs >  AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>=( const AquaFixed<bits, mulP, divP>& lhs, float_t rhs )					{ return lhs >= AquaFixed<bits, mulP, divP>( rhs );	}

	// double_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator==( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )				{ return AquaFixed<bits, mulP, divP>( lhs ) == rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator!=( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )				{ return AquaFixed<bits, mulP, divP>( lhs ) != rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs )  < rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<=( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )				{ return AquaFixed<bits, mulP, divP>( lhs ) <= rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )					{ return AquaFixed<bits, mulP, divP>( lhs )  > rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>=( double_t lhs, const AquaFixed<bits, mulP, divP>& rhs )				{ return AquaFixed<bits, mulP, divP>( lhs ) >= rhs;	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator==( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )				{ return lhs == AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator!=( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )				{ return lhs != AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )					{ return lhs <  AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator<=( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )				{ return lhs <= AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )					{ return lhs >  AquaFixed<bits, mulP, divP>( rhs );	}
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline bool_t operator>=( const AquaFixed<bits, mulP, divP>& lhs, double_t rhs )				{ return lhs >= AquaFixed<bits, mulP, divP>( rhs );	}


	//////////////////////////////////////////////////////////////////////////
	// Other assignments
	//////////////////////////////////////////////////////////////////////////

	// int32_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline int32_t& operator+=( int32_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs + rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline int32_t& operator-=( int32_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs - rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline int32_t& operator*=( int32_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs * rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline int32_t& operator/=( int32_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs / rhs; return lhs; }

	// float_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline float_t& operator+=( float_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs + rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline float_t& operator-=( float_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs - rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline float_t& operator*=( float_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs * rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline float_t& operator/=( float_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs / rhs; return lhs; }

	// double_t
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline double_t& operator+=( double_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs + rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline double_t& operator-=( double_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs - rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline double_t& operator*=( double_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs * rhs; return lhs; }
	template<int32_t bits, template <int32_t> class mulP, template <int32_t> class divP>	inline double_t& operator/=( double_t& lhs, const AquaFixed<bits, mulP, divP>& rhs)		{ lhs = (AquaFixed<bits, mulP, divP>)lhs / rhs; return lhs; }



	// local helpers
	inline int32_t neg( int32_t r )		{ return (r >> 31); }


	///////////////////////////////////////////////////////////////////////
	// High-precision policies (int64_t)
	///////////////////////////////////////////////////////////////////////

	template<int32_t bits>
	class HighPrecision : public AquaFixedInfo<bits>
	{
		public:

			inline static int32_t mul( int32_t l, int32_t r )
			{
				int64_t t = int64_t(l) * int64_t(r);
				t += static_cast<int32_t>( ROUND );
				t >>= bits;
				return int32_t(t);

/*
				int64_t tmp32_32;
				int32_t res16_16;
				tmp32_32 = l;
				tmp32_32 *= r;
				// result is now 32:32
				tmp32_32 >>= 16; // chop off the lower 16 bits
				res16_16 = ( int ) tmp32_32; // chop off the upper 16bits.
				// result is now back at 16:16
				return res16_16;
*/
			}

			inline static int32_t div( int32_t l, int32_t r )
			{
				int64_t t = int64_t(l) << bits;
				int32_t q = int32_t( t / r );
				int32_t rem = int32_t( t % r );
				q += 1 + neg( (int32_t)( (rem << 1 ) - r ) );
				return q;
			}

/*
			inline static int32_t mul( int32_t l, int32_t r )
			{
				int64_t t = int64_t(l) * int64_t(r);
				t += static_cast<int32_t>( ROUND );
				t >>= bits;
				return int32_t(t);
			}

			inline static int32_t div( int32_t l, int32_t r )
			{
				int64_t t = int64_t(l) << bits;
				t /= r;
				return int32_t(t);
			}
*/
	};


	///////////////////////////////////////////////////////////////////////
	// Mid-precision policies (int32_t)		*UNSAFE*
	///////////////////////////////////////////////////////////////////////

	// at now this is UNSAFE if going to overflow
	template<int32_t bits>
	class MidPrecision : public AquaFixedInfo<bits>
	{
		public:

			inline static int32_t mul( int32_t l, int32_t r )
			{
				register unsigned int a,b;
				register bool sign;

				int32_t a1 = l, b1 = r;

				sign = (a1 ^ b1) < 0;
				if (a1 < 0) {a1 = -a1;}
				if (b1 < 0) {b1 = -b1;}
				// a and b contain integer part
				// a1 and b1 contain fractional part.
				a = (((unsigned int)a1) >> bits); a1 &= ~(a << bits);
				b = (((unsigned int)b1) >> bits); b1 &= ~(b << bits);
				a1 = ((a*b) << bits) + (a*b1 + b*a1) +
					((unsigned int)((a1*b1) + (1 << (bits-1))) >> bits);

				if (a1 < 0) {a1 ^= static_cast<int32_t>( SIGN_BIT );}
				if (sign) {a1 = -a1;}
				return a1;
			}

			inline static int32_t div( int32_t l, int32_t r )
			{
				register int res, mask;
				register bool sign;

				int32_t a = l, b = r;

				sign = (a ^ b) < 0;
				if (a < 0) {a = -a;}
				if (b < 0) {b = -b;}
				mask = (1<<bits); res = 0;
				while (a > b) {b <<= 1; mask <<= 1;}
				//while (mask != 0)
				while (mask > 0)
				{
					if (a >= b) {res |= mask; a -= b;}
					mask >>= 1; b >>= 1;
				}

				if (res < 0) {res ^= static_cast<int32_t>( SIGN_BIT );}
				if (sign) {res = -res;}
				return res;
			}

	};


	///////////////////////////////////////////////////////////////////////
	// Low-precision policies (int32_t)
	///////////////////////////////////////////////////////////////////////

	template<int32_t bits>
	class LowPrecision : public AquaFixedInfo<bits>
	{
		public:

			inline static int32_t mul( int32_t l, int32_t r )
			{
				return ( ( l * r ) >> bits );
			}

			inline static int32_t div( int32_t l, int32_t r )
			{
				return( ( l << bits ) / r );
			}

	};


	// Predefines
	typedef AquaFixed<  8, LowPrecision,  LowPrecision  > fixed8_t;
	typedef AquaFixed< 16, HighPrecision, HighPrecision > fixed16_t;
	typedef fixed16_t fixed_t;

}	// end of namespace Aqua


#endif	// AQUA_FIXEDPOINT_H
