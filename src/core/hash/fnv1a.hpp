#ifndef CS2_CHEAT_FNV1A_HPP
#define CS2_CHEAT_FNV1A_HPP

#include <cstdint>
#include <string_view>

namespace Core::Hash
{
	using FNV1A_t = std::uint64_t;

	static constexpr FNV1A_t FNV_PRIME = 0x100000001B3;
	static constexpr FNV1A_t FNV_BASIS = 0xCBF29CE484222325;

	template< typename tChar >
	[[nodiscard]] constexpr FNV1A_t FNV1A( std::basic_string_view< tChar > szString, FNV1A_t uValue = FNV_BASIS ) noexcept
	{
		if ( szString.empty( ) )
			return uValue;

		for ( const auto itChar : szString )
		{
			uValue ^= itChar;
			uValue *= FNV_PRIME;
		}

		return uValue;
	}

	template< typename tChar >
	[[nodiscard]] constexpr FNV1A_t FNV1A( tChar* szString, FNV1A_t fnvValue = FNV_BASIS ) noexcept
	{
		return FNV1A( std::basic_string_view< tChar >( szString ), fnvValue );
	}
} // namespace Core::Hash

consteval Core::Hash::FNV1A_t operator""_FNV1A( const char* szString, std::size_t uLength ) noexcept
{
	return Core::Hash::FNV1A( std::string_view( szString, uLength ) );
}

consteval Core::Hash::FNV1A_t operator""_FNV1A( const wchar_t* szString, std::size_t uLength ) noexcept
{
	return Core::Hash::FNV1A( std::wstring_view( szString, uLength ) );
}

#endif // CS2_CHEAT_FNV1A_HPP
