/*
 * File:	StrPool.cpp
 * Author:	Gowtham Kudupudi
 */
#include "StrPool.h"
#include <cstring>
#include <limits>
#include <cstdint>
#ifdef __linux__
#include <link.h>
#endif
#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach/mach.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

StrPool_ StrPool;

#ifdef __linux__
struct ReadOnlyAddressData_ {
	uintptr_t _start;
	uintptr_t _end;
	bool _found;
};

static int readOnlyAddressCallback (struct dl_phdr_info* info, size_t, void* data) {
	ReadOnlyAddressData_* d = (ReadOnlyAddressData_*) data;
	for (int i = 0; i < info->dlpi_phnum; i++) {
		const ElfW(Phdr)& phdr = info->dlpi_phdr[i];
		if (phdr.p_type != PT_LOAD)
			continue;
		if (phdr.p_flags & PF_W)
			continue;
		uintptr_t start = info->dlpi_addr + phdr.p_vaddr;
		uintptr_t end = start + phdr.p_memsz;
		if (d->_start >= start && d->_end <= end) {
			d->_found = true;
			return 1;
		}
	}
	return 0;
}

static bool isReadOnlyAddress (ccp string, size_t size) {
	uintptr_t start = (uintptr_t) string;
	uintptr_t end = start + size;
	if (end < start)
		return false;
	ReadOnlyAddressData_ data;
	data._start = start;
	data._end = end;
	data._found = false;
	dl_iterate_phdr(readOnlyAddressCallback, &data);
	return data._found;
}
#elif defined(__APPLE__)
static bool isReadOnlyAddress (ccp string, size_t size) {
	uintptr_t start = (uintptr_t) string;
	uintptr_t end = start + size;
	if (end < start)
		return false;
	uint32_t imageCount = _dyld_image_count();
	for (uint32_t imageIndex = 0; imageIndex < imageCount; imageIndex++) {
		const struct mach_header* header = _dyld_get_image_header(imageIndex);
		if (!header)
			continue;
		uintptr_t slide = (uintptr_t) _dyld_get_image_vmaddr_slide(imageIndex);
		if (header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64) {
			const struct mach_header_64* header64 =
				(const struct mach_header_64*) header;
			const struct load_command* command =
				(const struct load_command*) ((const char*) header64 +
				sizeof(struct mach_header_64));
			for (uint32_t i = 0; i < header64->ncmds; i++) {
				if (command->cmd == LC_SEGMENT_64) {
					const struct segment_command_64* segment =
						(const struct segment_command_64*) command;
					if (!(segment->initprot & VM_PROT_WRITE)) {
						uintptr_t segmentStart =
							slide + segment->vmaddr;
						uintptr_t segmentEnd =
							segmentStart + segment->vmsize;
						if (start >= segmentStart && end <= segmentEnd)
							return true;
					}
				}
				command = (const struct load_command*)
					((const char*) command + command->cmdsize);
			}
		}
		else if (header->magic == MH_MAGIC || header->magic == MH_CIGAM) {
			const struct load_command* command =
				(const struct load_command*) ((const char*) header +
				sizeof(struct mach_header));
			for (uint32_t i = 0; i < header->ncmds; i++) {
				if (command->cmd == LC_SEGMENT) {
					const struct segment_command* segment =
						(const struct segment_command*) command;
					if (!(segment->initprot & VM_PROT_WRITE)) {
						uintptr_t segmentStart =
							slide + segment->vmaddr;
						uintptr_t segmentEnd =
							segmentStart + segment->vmsize;
						if (start >= segmentStart && end <= segmentEnd)
							return true;
					}
				}
				command = (const struct load_command*)
					((const char*) command + command->cmdsize);
			}
		}
	}
	return false;
}
#elif defined(_WIN32)
static bool isReadOnlyAddress (ccp string, size_t size) {
	uintptr_t start = (uintptr_t) string;
	uintptr_t end = start + size;
	if (end < start)
		return false;
	uintptr_t address = start;
	while (address < end) {
		MEMORY_BASIC_INFORMATION info;
		if (!VirtualQuery(
			(const void*) address,
			&info,
			sizeof(info)
		))
			return false;
		if (info.State != MEM_COMMIT)
			return false;
		DWORD protection = info.Protect & 0xff;
		if (protection != PAGE_READONLY &&
			protection != PAGE_EXECUTE_READ)
			return false;
		uintptr_t regionStart = (uintptr_t) info.BaseAddress;
		uintptr_t regionEnd = regionStart + info.RegionSize;
		if (regionEnd <= address)
			return false;
		address = regionEnd;
	}
	return true;
}
#else
static bool isReadOnlyAddress (ccp, size_t) {
	return false;
}
#endif

StrPool_::Arena_::Arena_ (size_t size) {
	_data = new char[size];
	_size = size;
	_used = 0;
}

StrPool_::Arena_::~Arena_ () {
	delete[] _data;
}

bool StrPool_::Arena_::hasSpace (size_t size) const {
	return size <= _size - _used;
}

char* StrPool_::Arena_::allocate (size_t size) {
	if (!hasSpace(size))
		return nullptr;
	char* data = _data + _used;
	_used += size;
	return data;
}

size_t StrPool_::Hash_::operator () (ccp string) const {
	size_t hash = 1469598103934665603ULL;
	while (*string) {
		hash ^= (unsigned char) *string++;
		hash *= 1099511628211ULL;
	}
	return hash;
}

bool StrPool_::Equal_::operator () (ccp a, ccp b) const {
	return strcmp(a, b) == 0;
}

StrPool_::StrPool_ (size_t arenaSize) {
	if (!arenaSize)
		arenaSize = 4096;
	_initialArenaSize = arenaSize;
	_nextArenaSize = arenaSize;
}

StrPool_::~StrPool_ () {
	clear();
}

char* StrPool_::allocate (size_t size) {
	if (!_arenas.empty() && _arenas.back()->hasSpace(size))
		return _arenas.back()->allocate(size);
	size_t arenaSize = _nextArenaSize;
	if (arenaSize < size)
		arenaSize = size;
	Arena_* arena = new Arena_(arenaSize);
	_arenas.push_back(arena);
	if (_nextArenaSize <= numeric_limits<size_t>::max() / 2)
		_nextArenaSize *= 2;
	else
		_nextArenaSize = numeric_limits<size_t>::max();
	return arena->allocate(size);
}

ccp StrPool_::ccpFromPool (ccp string) {
	if (!string)
		return nullptr;
	size_t size = strlen(string) + 1;
	if (size<=1)
		return nullptr;
	auto i = _strings.find(string);
	if (i != _strings.end())
		return *i;
	if (isReadOnlyAddress(string, size)) {
		_strings.insert(string);
		return string;
	}
	char* pooledString = allocate(size);
	memcpy(pooledString, string, size);
	_strings.insert(pooledString);
	return pooledString;
}

ccp StrPool_::ccpIfInPool (ccp string) {
	if (!string || !strlen(string))
		return nullptr;

	auto i = _strings.find(string);

	if (i == _strings.end())
		return nullptr;

	return *i;
}

void StrPool_::clear () {
	_strings.clear();
	for (Arena_* arena : _arenas)
		delete arena;
	_arenas.clear();
	_nextArenaSize = _initialArenaSize;
}

ccp ccpFromPool (ccp string) {
	return StrPool.ccpFromPool(string);
}

ccp ccpFromPool (const std::string& string) {
	return StrPool.ccpFromPool(string.c_str());
}

ccp ccpIfInPool (ccp string) {
	return StrPool.ccpIfInPool(string);
}

ccp ccpIfInPool (const std::string& str) {
	return StrPool.ccpIfInPool(str.c_str());
}
