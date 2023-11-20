#ifndef CS2_CHEAT_SCHEMA_HPP
#define CS2_CHEAT_SCHEMA_HPP

#include <memory>
#include <cstdint>
#include <unordered_map>

#include "../hash/fnv1a.hpp"

namespace Core
{
	class CSchema
	{
	public:
		CSchema( );
		~CSchema( ) noexcept = default;

		[[nodiscard]] std::uint32_t GetOffset( Hash::FNV1A_t fnvFieldHashed ) const noexcept;

	private:
		std::unordered_map< Hash::FNV1A_t, std::uint32_t > m_mapOffsets = { };
	};

	inline std::unique_ptr< CSchema > pSchema = nullptr;
} // namespace Core

#define SCHEMA_VARIABLE_OFFSET( szFieldName, pFunction, iAdditional, tReturn, ... )                                                                                                  \
	[[nodiscard]] auto pFunction( ) noexcept                                                                                                                                         \
	{                                                                                                                                                                                \
		static std::uint32_t uOffset = Core::pSchema->GetOffset( Core::Hash::FNV1A( szFieldName ) );                                                                                 \
		return *reinterpret_cast< std::add_pointer_t< tReturn __VA_OPT__(, ) __VA_ARGS__ > >( reinterpret_cast< std::uintptr_t >( this ) + uOffset + iAdditional );                  \
	}                                                                                                                                                                                \
	[[nodiscard]] auto pFunction( ) const noexcept                                                                                                                                   \
	{                                                                                                                                                                                \
		static std::uint32_t uOffset = Core::pSchema->GetOffset( Core::Hash::FNV1A( szFieldName ) );                                                                                 \
		return std::as_const( *reinterpret_cast< std::add_pointer_t< tReturn __VA_OPT__(, ) __VA_ARGS__ > >( reinterpret_cast< std::uintptr_t >( this ) + uOffset + iAdditional ) ); \
	}

#define SCHEMA_VARIABLE( szFieldName, pFunction, tReturn, ... ) \
	SCHEMA_VARIABLE_OFFSET( szFieldName, pFunction, 0, tReturn, __VA_ARGS__ )

#endif // CS2_CHEAT_SCHEMA_HPP
