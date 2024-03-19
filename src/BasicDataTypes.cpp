/* Reverse Engineer's Hex Editor
 * Copyright (C) 2020-2024 Daniel Collins <solemnwarning@solemnwarning.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "platform.hpp"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "BasicDataTypes.hpp"
#include "BitOffset.hpp"
#include "DataType.hpp"
#include "document.hpp"
#include "DocumentCtrl.hpp"
#include "NumericTextCtrl.hpp"
#include "SharedDocumentPointer.hpp"

/* This MUST come after the wxWidgets headers have been included, else we pull in windows.h BEFORE the wxWidgets
 * headers when building on Windows and this causes unicode-flavoured pointer conversion errors.
*/
#include "endian_conv.hpp"

#define IMPLEMENT_NDTR_CLASS(NAME, T, LABEL, FMT, XTOH, HTOX, FACTORY_FUNC) \
	REHex::NAME::NAME(SharedDocumentPointer &doc, REHex::BitOffset offset, REHex::BitOffset length, REHex::BitOffset virt_offset): \
		NumericDataTypeRegion(doc, offset, length, virt_offset, LABEL) {} \
	\
	std::string REHex::NAME::to_string(const T *data) const \
	{ \
		char buf[128]; \
		snprintf(buf, sizeof(buf), FMT, (T)(XTOH(*data))); \
		\
		return std::string(buf); \
	} \
	\
	bool REHex::NAME::write_string_value(const std::string &value) \
	{ \
		T buf; \
		try { \
			buf = NumericTextCtrl::ParseValue<T>(value); \
		} \
		catch(const REHex::NumericTextCtrl::InputError &e) \
		{ \
			return false; \
		} \
		buf = HTOX(buf); \
		doc->overwrite_data(d_offset.byte(), &buf, sizeof(buf)); /* BITFIXUP */ \
		return true; \
	} \
	\
	static REHex::DocumentCtrl::Region *FACTORY_FUNC(REHex::SharedDocumentPointer &doc, REHex::BitOffset offset, REHex::BitOffset length, REHex::BitOffset virt_offset) \
	{ \
		return new REHex::NAME(doc, offset, length, virt_offset); \
	}

IMPLEMENT_NDTR_CLASS(U8DataRegion, uint8_t, "u8", "%" PRIu8, (uint8_t), (uint8_t), u8_factory)
IMPLEMENT_NDTR_CLASS(S8DataRegion, int8_t, "s8", "%" PRId8, (int8_t), (int8_t), s8_factory)

static REHex::StaticDataTypeRegistration u8_dtr(
	"u8", "unsigned 8-bit", { "Number" },
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(uint8_t), 0))
		.WithFixedSizeRegion(&u8_factory, REHex::BitOffset(sizeof(uint8_t), 0)));

static REHex::StaticDataTypeRegistration s8_dtr(
	"s8", "signed 8-bit", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(int8_t), 0))
		.WithFixedSizeRegion(&s8_factory, REHex::BitOffset(sizeof(int8_t), 0)));

IMPLEMENT_NDTR_CLASS(U16LEDataRegion, uint16_t, "u16le", "%" PRIu16, le16toh, htole16, u16le_factory)
IMPLEMENT_NDTR_CLASS(U16BEDataRegion, uint16_t, "u16be", "%" PRIu16, be16toh, htobe16, u16be_factory)
IMPLEMENT_NDTR_CLASS(S16LEDataRegion, int16_t,  "s16le", "%" PRId16, le16toh, htole16, s16le_factory)
IMPLEMENT_NDTR_CLASS(S16BEDataRegion, int16_t,  "s16be", "%" PRId16, be16toh, htobe16, s16be_factory)

static REHex::StaticDataTypeRegistration u16le_dtr(
	"u16le", "unsigned 16-bit (little endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(uint16_t), 0))
		.WithFixedSizeRegion(&u16le_factory, REHex::BitOffset(sizeof(uint16_t), 0)));

static REHex::StaticDataTypeRegistration u16be_dtr(
	"u16be", "unsigned 16-bit (big endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(uint16_t), 0))
		.WithFixedSizeRegion(&u16be_factory, REHex::BitOffset(sizeof(uint16_t), 0)));

static REHex::StaticDataTypeRegistration s16le_dtr(
	"s16le", "signed 16-bit (little endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(int16_t), 0))
		.WithFixedSizeRegion(&s16le_factory, REHex::BitOffset(sizeof(int16_t), 0)));

static REHex::StaticDataTypeRegistration s16be_dtr(
	"s16be", "signed 16-bit (big endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(int16_t), 0))
		.WithFixedSizeRegion(&s16be_factory, REHex::BitOffset(sizeof(int16_t), 0)));

IMPLEMENT_NDTR_CLASS(U32LEDataRegion, uint32_t, "u32le", "%" PRIu32, le32toh, htole32, u32le_factory)
IMPLEMENT_NDTR_CLASS(U32BEDataRegion, uint32_t, "u32be", "%" PRIu32, be32toh, htobe32, u32be_factory)
IMPLEMENT_NDTR_CLASS(S32LEDataRegion, int32_t,  "s32le", "%" PRId32, le32toh, htole32, s32le_factory)
IMPLEMENT_NDTR_CLASS(S32BEDataRegion, int32_t,  "s32be", "%" PRId32, be32toh, htobe32, s32be_factory)

static REHex::StaticDataTypeRegistration u32le_dtr(
	"u32le", "unsigned 32-bit (little endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(uint32_t), 0))
		.WithFixedSizeRegion(&u32le_factory, REHex::BitOffset(sizeof(uint32_t))));

static REHex::StaticDataTypeRegistration u32be_dtr(
	"u32be", "unsigned 32-bit (big endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(uint32_t), 0))
		.WithFixedSizeRegion(&u32be_factory, REHex::BitOffset(sizeof(uint32_t), 0)));

static REHex::StaticDataTypeRegistration s32le_dtr(
	"s32le", "signed 32-bit (little endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(int32_t), 0))
		.WithFixedSizeRegion(&s32le_factory, REHex::BitOffset(sizeof(int32_t), 0)));

static REHex::StaticDataTypeRegistration s32be_dtr(
	"s32be", "signed 32-bit (big endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(int32_t), 0))
		.WithFixedSizeRegion(&s32be_factory, REHex::BitOffset(sizeof(int32_t), 0)));

IMPLEMENT_NDTR_CLASS(U64LEDataRegion, uint64_t, "u64le", "%" PRIu64, le64toh, htole64, u64le_factory)
IMPLEMENT_NDTR_CLASS(U64BEDataRegion, uint64_t, "u64be", "%" PRIu64, be64toh, htobe64, u64be_factory)
IMPLEMENT_NDTR_CLASS(S64LEDataRegion, int64_t,  "s64le", "%" PRId64, le64toh, htole64, s64le_factory)
IMPLEMENT_NDTR_CLASS(S64BEDataRegion, int64_t,  "s64be", "%" PRId64, be64toh, htobe64, s64be_factory)

static REHex::StaticDataTypeRegistration u64le_dtr(
	"u64le", "unsigned 64-bit (little endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(uint64_t), 0))
		.WithFixedSizeRegion(&u64le_factory, REHex::BitOffset(sizeof(uint64_t), 0)));

static REHex::StaticDataTypeRegistration u64be_dtr(
	"u64be", "unsigned 64-bit (big endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(uint64_t), 0))
		.WithFixedSizeRegion(&u64be_factory, REHex::BitOffset(sizeof(uint64_t), 0)));

static REHex::StaticDataTypeRegistration s64le_dtr(
	"s64le", "signed 64-bit (little endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(int64_t), 0))
		.WithFixedSizeRegion(&s64le_factory, REHex::BitOffset(sizeof(int64_t), 0)));

static REHex::StaticDataTypeRegistration s64be_dtr(
	"s64be", "signed 64-bit (big endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(int64_t), 0))
		.WithFixedSizeRegion(&s64be_factory, REHex::BitOffset(sizeof(int64_t), 0)));

#define IMPLEMENT_NDTR_CLASS_FLOAT(NAME, T, LABEL, FMT, XTOH, HTOX, FACTORY_FUNC) \
	REHex::NAME::NAME(SharedDocumentPointer &doc, REHex::BitOffset offset, REHex::BitOffset length, REHex::BitOffset virt_offset): \
		NumericDataTypeRegion(doc, offset, length, virt_offset, LABEL) {} \
	\
	std::string REHex::NAME::to_string(const T *data) const \
	{ \
		char buf[128]; \
		snprintf(buf, sizeof(buf), FMT, XTOH<T>(*data)); \
		\
		return std::string(buf); \
	} \
	\
	bool REHex::NAME::write_string_value(const std::string &value) \
	{ \
		if(value.length() == 0) \
		{ \
			return false; \
		} \
		\
		errno = 0; \
		char *endptr; \
		\
		T buf = strtof(value.c_str(), &endptr); \
		if(*endptr != '\0') \
		{ \
			return false; \
		} \
		if((buf == HUGE_VALF || buf == -HUGE_VALF) && errno == ERANGE) \
		{ \
			return false; \
		} \
		\
		buf = HTOX<T>(buf); \
		doc->overwrite_data(d_offset.byte(), &buf, sizeof(buf)); /* BITFIXUP */ \
		return true; \
	} \
	\
	static REHex::DocumentCtrl::Region *FACTORY_FUNC(REHex::SharedDocumentPointer &doc, REHex::BitOffset offset, REHex::BitOffset length, REHex::BitOffset virt_offset) \
	{ \
		return new REHex::NAME(doc, offset, length, virt_offset); \
	}

IMPLEMENT_NDTR_CLASS_FLOAT(F32LEDataRegion, float,  "f32le", "%.9g", leXXXtoh, htoleXXX, f32le_factory)
IMPLEMENT_NDTR_CLASS_FLOAT(F32BEDataRegion, float,  "f32be", "%.9g", beXXXtoh, htobeXXX, f32be_factory)

static REHex::StaticDataTypeRegistration f32le_dtr(
	"f32le", "32-bit float (little endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(float), 0))
		.WithFixedSizeRegion(&f32le_factory, REHex::BitOffset(sizeof(float), 0)));

static REHex::StaticDataTypeRegistration f32be_dtr(
	"f32be", "32-bit float (big endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(float), 0))
		.WithFixedSizeRegion(&f32be_factory, REHex::BitOffset(sizeof(float), 0)));

#define IMPLEMENT_NDTR_CLASS_DOUBLE(NAME, T, LABEL, FMT, XTOH, HTOX, FACTORY_FUNC) \
	REHex::NAME::NAME(SharedDocumentPointer &doc, REHex::BitOffset offset, REHex::BitOffset length, REHex::BitOffset virt_offset): \
		NumericDataTypeRegion(doc, offset, length, virt_offset, LABEL) {} \
	\
	std::string REHex::NAME::to_string(const T *data) const \
	{ \
		char buf[128]; \
		snprintf(buf, sizeof(buf), FMT, XTOH<T>(*data)); \
		\
		return std::string(buf); \
	} \
	\
	bool REHex::NAME::write_string_value(const std::string &value) \
	{ \
		if(value.length() == 0) \
		{ \
			return false; \
		} \
		\
		errno = 0; \
		char *endptr; \
		\
		T buf = strtod(value.c_str(), &endptr); \
		if(*endptr != '\0') \
		{ \
			return false; \
		} \
		if((buf == HUGE_VAL || buf == -HUGE_VAL) && errno == ERANGE) \
		{ \
			return false; \
		} \
		\
		buf = HTOX<T>(buf); \
		doc->overwrite_data(d_offset.byte(), &buf, sizeof(buf)); /* BITFIXUP */ \
		return true; \
	} \
	\
	static REHex::DocumentCtrl::Region *FACTORY_FUNC(REHex::SharedDocumentPointer &doc, REHex::BitOffset offset, REHex::BitOffset length, REHex::BitOffset virt_offset) \
	{ \
		return new REHex::NAME(doc, offset, length, virt_offset); \
	}

IMPLEMENT_NDTR_CLASS_DOUBLE(F64LEDataRegion, double, "f64le", "%.9g", leXXXtoh, htoleXXX, f64le_factory)
IMPLEMENT_NDTR_CLASS_DOUBLE(F64BEDataRegion, double, "f64be", "%.9g", beXXXtoh, htobeXXX, f64be_factory)

static REHex::StaticDataTypeRegistration f64le_dtr(
	"f64le", "64-bit float (double) (little endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(double), 0))
		.WithFixedSizeRegion(&f64le_factory, REHex::BitOffset(sizeof(double), 0)));

static REHex::StaticDataTypeRegistration f64be_dtr(
	"f64be", "64-bit float (double) (big endian)", {"Number"},
	REHex::DataType()
		.WithWordSize(REHex::BitOffset(sizeof(double), 0))
		.WithFixedSizeRegion(&f64be_factory, REHex::BitOffset(sizeof(double), 0)));
