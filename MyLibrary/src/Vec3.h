#ifndef VEC_H_
#define VEC_H_

#include <iostream>

namespace lan {

// Vec3 m_Data offsets
static constexpr int _X = 0;
static constexpr int _Y = 1;
static constexpr int _Z = 2;

// ValBase
struct DoubleVec3Value {
	double m_Data[3];
};

template <typename T>
class Vec3;
template <>
class Vec3<double>;

template <typename T, class ValBase>
class Vec3Base : public ValBase {
public:
	using ValueType = T;

#ifdef DEBUG_LOGGING
	Vec3Base(const T& Xval, const T& Yval, const T& Zval)
		: ValBase{ { Xval, Yval, Zval } } {
		std::cout << "Vec3Base construct!\n";
	}
#else
	constexpr Vec3Base(const T& Xval, const T& Yval, const T& Zval)
		: ValBase{ { Xval, Yval, Zval } } {}
#endif // DEBUG_LOGGING

	inline void setX(const T& Xval) {
		this->m_Data[_X] = Xval;
	}

	inline void setY(const T& Yval) {
		this->m_Data[_Y] = Yval;
	}

	inline void setZ(const T& Zval) {
		this->m_Data[_Z] = Zval;
	}

	constexpr T getX() const {
		return this->m_Data[_X];
	}

	constexpr T getY() const {
		return this->m_Data[_Y];
	}

	constexpr T getZ() const {
		return this->m_Data[_Z];
	}

protected:
	template <class Other>
	inline void m_Add(const Vec3<Other>& Right) {
		this->m_Data[_X] = this->m_Data[_X] + static_cast<T>(Right.getX());
		this->m_Data[_Y] = this->m_Data[_Y] + static_cast<T>(Right.getY());
		this->m_Data[_Z] = this->m_Data[_Z] + static_cast<T>(Right.getZ());
	}

	template <class Other>
	inline void m_Sub(const Vec3<Other>& Right) {
		this->m_Data[_X] = this->m_Data[_X] - static_cast<T>(Right.getX());
		this->m_Data[_Y] = this->m_Data[_Y] - static_cast<T>(Right.getY());
		this->m_Data[_Z] = this->m_Data[_Z] - static_cast<T>(Right.getZ());
	}

	template <class Other>
	inline void m_Cross(const Vec3<Other>& Right) {
		T RightX = static_cast<T>(Right.getX());
		T RightY = static_cast<T>(Right.getY());
		T RightZ = static_cast<T>(Right.getZ());
		T LeftX = this->m_Data[_X];
		T LeftY = this->m_Data[_Y];
		T LeftZ = this->m_Data[_Z];
		this->m_Data[_X] = LeftY * RightZ - LeftZ * RightY;
		this->m_Data[_Y] = LeftZ * RightX - LeftX * RightZ;
		this->m_Data[_Z] = LeftX * RightY -	LeftY * RightX;
	}
};

template <>
class Vec3<double> : public Vec3Base<double, DoubleVec3Value> {
public:
	using T = double;

#ifdef DEBUG_LOGGING
	Vec3(const T& Xval = 0, const T& Yval = 0, const T& Zval = 0)
		: Vec3Base<double, DoubleVec3Value>(Xval, Yval, Zval) {
		std::cout << "Vec3 pararmeter construct!\n";
	}
#else
	constexpr Vec3(const T& Xval = 0, const T& Yval = 0, const T& Zval = 0)
		: Vec3Base<double, DoubleVec3Value>(Xval, Yval, Zval) {}
#endif // DEBUG_LOGGING

#ifdef DEBUG_LOGGING
	Vec3(const Vec3<T>& Other)
		: Vec3Base<double, DoubleVec3Value>(Other.getX(), Other.getY(), Other.getZ()) {
		std::cout << "Vec3 copy construct!\n";
	}

	Vec3(Vec3<T>&& Other) noexcept
		: Vec3Base<double, DoubleVec3Value>(Other.getX(), Other.getY(), Other.getZ()) {
		std::cout << "Vec3 move construct!\n";
	}

	~Vec3() {
		std::cout << "Vec3 destroy!\n";
	}
#endif // DEBUG_LOGGING

	inline Vec3& operator+=(const Vec3& Right) {
		this->m_Add(Right);
		return *this;
	}

	inline Vec3& operator-=(const Vec3& Right) {
		this->m_Sub(Right);
		return *this;
	}

	Vec3& operator*=(const Vec3&) = delete;
	Vec3& operator/=(const Vec3&) = delete;

	template <typename Other>
	inline Vec3& operator=(const Vec3<Other>& Right) {
#ifdef DEBUG_LOGGING
		std::cout << "Vec3 copy assignment!\n";
#endif // DEBUG_LOGGING
		this->m_Data[_X] = static_cast<T>(Right.getX());
		this->m_Data[_Y] = static_cast<T>(Right.getY());
		this->m_Data[_Z] = static_cast<T>(Right.getZ());
		return *this;
	}

#ifdef DEBUG_LOGGING
	inline Vec3& operator=(const Vec3& Right) {
		std::cout << "Vec3 copy assignment!\n";
		this->m_Data[_X] = Right.getX();
		this->m_Data[_Y] = Right.getY();
		this->m_Data[_Z] = Right.getZ();
		return *this;
	}

	inline Vec3& operator=(Vec3&& Right) noexcept {
		std::cout << "Vec3 move assignment!\n";
		this->m_Data[_X] = Right.getX();
		this->m_Data[_Y] = Right.getY();
		this->m_Data[_Z] = Right.getZ();
		return *this;
	}
#endif // DEBUG_LOGGING

	inline Vec3& cross(const Vec3& Right) {
		this->m_Cross(Right);
		return *this;
	}
};

template <typename T>
inline Vec3<T> operator+(const Vec3<T>& Left, const Vec3<T>& Right) {
	Vec3<T> Temp(Left);
	Temp += Right;
	return Temp;
}

template <typename T>
inline Vec3<T> operator-(const Vec3<T>& Left, const Vec3<T>& Right) {
	Vec3<T> Temp(Left);
	Temp -= Right;
	return Temp;
}

template <typename T>
constexpr T getX(const Vec3<T>& Left) {
	return Left.getX();
}

template <typename T>
constexpr T getY(const Vec3<T>& Left) {
	return Left.getY();
}

template <typename T>
constexpr T getZ(const Vec3<T>& Left) {
	return Left.getZ();
}

template <typename T>
constexpr T dot(const Vec3<T>& Left, const Vec3<T>& Right) {
	return getX(Left) * getX(Right) + getY(Left) * getY(Right) + getZ(Left) * getZ(Right);
}

template <typename T>
inline Vec3<T> cross(const Vec3<T>& Left, const Vec3<T>& Right) {
	Vec3<T> Temp(Left);
	Temp.cross(Right);
	return Temp;
}

template <typename T>
inline Vec3<T>& cross(const Vec3<T>& Left, const Vec3<T>& Right, Vec3<T>& Result) {
	Result = Left;
	Result.cross(Right);
	return Result;
}

template <typename T, typename Elem, class Tr>
std::basic_istream<Elem, Tr>& operator>>(std::basic_istream<Elem, Tr>& Istr, Vec3<T>& Right) {
	const std::ctype<Elem>& Ctype_fac = std::use_facet<std::ctype<Elem>>(Istr.getloc());
	Elem Ch = 0;
	long double Xval = 0;
	long double Yval = 0;
	long double Zval = 0;

	if (Istr >> Ch && Ch != Ctype_fac.widen('(')) { // no leading '(', treat as bad field
		Istr.putback(Ch);
		Istr.setstate(std::ios_base::failbit);
	} else if (Istr >> Xval >> Ch && Ch != Ctype_fac.widen(',')) {
		if (Ch == Ctype_fac.widen(')')) { // (Xval)
			Yval = 0;
			Zval = 0;
		} else { // no trailing ')' after Xval, treat as bad field
			Istr.putback(Ch);
			Istr.setstate(std::ios_base::failbit);
		}
	} else if (Istr >> Yval >> Ch && Ch != Ctype_fac.widen(',')) {
		if (Ch == Ctype_fac.widen(')')) { // (Xval, Yval)
			Zval = 0;
		} else { // no trailing ')' after Yval, treat as bad field
			Istr.putback(Ch);
			Istr.setstate(std::ios_base::failbit);
		}
	} else if (Istr >> Zval >> Ch && Ch != Ctype_fac.widen(')')) {
		// no Zval or trailing ')', treat as bad field
		Istr.putback(Ch);
		Istr.setstate(std::ios_base::failbit);
	}

	if (!Istr.fail()) { // store valid result
		T TypeX(static_cast<T>(Xval));
		T TypeY(static_cast<T>(Yval));
		T TypeZ(static_cast<T>(Zval));
		Right = Vec3<T>(TypeX, TypeY, TypeZ);
	}
	return Istr;
}

template <class T, class Elem, class Tr>
std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& Ostr, const Vec3<T>& Right) {
	const std::ctype<Elem>& Ctype_fac = std::use_facet<std::ctype<Elem>>(Ostr.getloc());
	std::basic_ostringstream<Elem, Tr, std::allocator<Elem>> Sstr;

	Sstr.flags(Ostr.flags());
	Sstr.imbue(Ostr.getloc());
	Sstr.precision(Ostr.precision());
	Sstr << Ctype_fac.widen('(') << getX(Right) << Ctype_fac.widen(',')
		<< getY(Right) << Ctype_fac.widen(',') << getZ(Right) << Ctype_fac.widen(')');

	return Ostr << Sstr.str();
}

} // namespace lan



#endif // !VEC_H_