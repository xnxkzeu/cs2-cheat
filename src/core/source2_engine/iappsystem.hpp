#ifndef CS2_CHEAT_IAPPSYSTEM_HPP
#define CS2_CHEAT_IAPPSYSTEM_HPP

struct IAppSystem
{
	IAppSystem( ) = delete;
	IAppSystem( IAppSystem&& ) = delete;
	IAppSystem( const IAppSystem& ) = delete;

	virtual bool Connect( void* pfnFactory ) noexcept = 0;
	virtual void Disconnect( ) noexcept = 0;
	[[nodiscard]] virtual void* QueryInterface( const char* szInterfaceName ) const noexcept = 0;
	virtual int Init( ) noexcept = 0;
	virtual void Shutdown( ) noexcept = 0;
	virtual void PreShutdown( ) noexcept = 0;
	[[nodiscard]] virtual void* GetDependencies( ) const noexcept = 0;
	[[nodiscard]] virtual int GetTier( ) const noexcept = 0;
	virtual void Reconnect( void* pfnFactory, const char* szInterfaceName ) noexcept = 0;
	[[nodiscard]] virtual bool IsSingleton( ) const noexcept = 0;
	[[nodiscard]] virtual int GetBuildType( ) const noexcept = 0;
};

#endif // CS2_CHEAT_IAPPSYSTEM_HPP
