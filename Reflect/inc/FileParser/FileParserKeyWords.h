#pragma once

namespace Reflect::Keys
{
	constexpr const char* ClassKey = "class";
	constexpr const char* StructKey = "struct";
	constexpr const char* ContainerKeys[] =
	{
		ClassKey, StructKey
	};

	constexpr const char* TypedefKey = "typedef";
	constexpr const char* UsingKey = "using";
	constexpr const char* AliasKeys[] =
	{
		TypedefKey, UsingKey
	};

	constexpr const char* PublicKey = "public";
	constexpr const char* ProtectedKey = "protected";
	constexpr const char* PrivateKey = "private";
	constexpr const char* VisabilityKeys[] = 
	{
		PublicKey, ProtectedKey, PrivateKey
	};

	constexpr char ReferenceKey = '&';
	constexpr char PointerKey = '*';
	constexpr char WhiteSpaceKey = ' ';

	constexpr const char* ConstKey = "const";
	constexpr const char* StaticKey = "static";
	constexpr const char* VolatileKey = "volatile";
	constexpr const char* VirtualKey = "virtual";
	constexpr const char* ValueModifierKeys[] =
	{
		ConstKey, StaticKey, VolatileKey, VirtualKey
	};

	constexpr const char* PointerTypeIdKey = "__ptr64";
	constexpr const char* TemplateKey = "template";
	constexpr const char* OperatorKey = "operator";
	constexpr const char* FriendKey = "friend";

	constexpr const char* Namespace = "namespace";

	constexpr const char* MetaPropertyKey = "Meta";
}