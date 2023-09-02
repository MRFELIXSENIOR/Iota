#include "TypeTraits.hpp"

using namespace iota;
using namespace Mono;

const TypeMap& Mono::GetTypeMap() {
	static const std::map<index_t, TypeName> map = {
		{ IndexOf<int8_t>(), { "sbyte", "System.SByte" }},
		{ IndexOf<uint8_t>(), { "byte", "System.Byte" }},
		{ IndexOf<int16_t>(), { "short", "System.Int16" }},
		{ IndexOf<uint16_t>(), { "ushort", "System.UInt16" }},
		{ IndexOf<int32_t>(), { "int", "System.Int32" }},
		{ IndexOf<uint32_t>(), { "uint", "System.UInt32" }},
		{ IndexOf<int64_t>(), { "long", "System.Int64" }},
		{ IndexOf<uint64_t>(), { "ulong", "System.UInt64" }},
		{ IndexOf<bool>(), { "bool", "System.Boolean" }},
		{ IndexOf<float>(), { "single", "System.Single" }},
		{ IndexOf<double>(), { "double", "System.Double" }},
		{ IndexOf<char16_t>(), { "char", "System.Char" }},
		{ IndexOf<std::string>(), { "string", "System.String" }},
		{ IndexOf<void>(), { "void", "System.Void" }}
	};

	return map;
}