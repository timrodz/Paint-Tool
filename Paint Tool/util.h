#ifndef __UTIL_H__
#define __UTIL_H__

template <class T>
std::string ToString(const T& _val) {

	std::strstream stream;
	stream << _val << std::ends;
	return (stream.str());

}

#endif //__UTIL_H__