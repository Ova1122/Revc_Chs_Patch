#include <MemUtils.h>

#include "pch.h"
#include <iostream>

#include <mutex>
#include <bitset>
#include <array> // Ensure this header is included for std::array

using namespace std;
//指令集加速
//#define AVX_SSE_ENABLE

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

bool FHexCharValid(char c)
{
	if (c >= '0' && c <= '9' ||
		c >= 'A' && c <= 'F' ||
		c >= 'a' && c <= 'f' ||
		c == '?')
		return true;
	else
		return false;
}

bool FHexDecoder(char* Dec, char* Src)
{
	char HighC, LowC;
	DWORD dwSrcLen = strlen(Src) / 2;
	int i;
	for (i = 0; i < dwSrcLen; i++) {
		HighC = Src[i * 2], LowC = Src[i * 2 + 1];
		if (!FHexCharValid(LowC) || !FHexCharValid(HighC))
			return false;
		HighC -= '0';
		if (HighC > 9) HighC -= 7;
		if (HighC > 0xf) HighC -= 0x20;
		LowC -= '0';
		if (LowC > 9) LowC -= 7;
		if (LowC > 0xf) LowC -= 0x20;
		Dec[i] = (HighC << 4) | LowC;
	}
	return true;
}


#ifdef AVX_SSE_ENABLE

#include <ppl.h>  // 并行模式库
#include <emmintrin.h> // 添加此头文件以解决未定义标识符 "__m128i" 和 "_mm_loadu_si128" 的问题

//= ============================================================================================ =
//==============================================================================================
// x86、x64处理器，使用__cpuid来获取有关CPU所支持的扩展指令集的信息
class InstructionSet
{
	class InstructionSet_Internal;  // 前向声明

public:
	// getters
	static std::string Vendor(void) { return CPU_Rep.vendor_; }
	static std::string Brand(void) { return CPU_Rep.brand_; }

	static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
	static bool PCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }
	static bool MONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }
	static bool SSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }
	static bool FMA(void) { return CPU_Rep.f_1_ECX_[12]; }
	static bool CMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }
	static bool SSE41(void) { return CPU_Rep.f_1_ECX_[19]; }
	static bool SSE42(void) { return CPU_Rep.f_1_ECX_[20]; }
	static bool MOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }
	static bool POPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }
	static bool AES(void) { return CPU_Rep.f_1_ECX_[25]; }
	static bool XSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }
	static bool OSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }
	static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }
	static bool F16C(void) { return CPU_Rep.f_1_ECX_[29]; }
	static bool RDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }

	static bool MSR(void) { return CPU_Rep.f_1_EDX_[5]; }
	static bool CX8(void) { return CPU_Rep.f_1_EDX_[8]; }
	static bool SEP(void) { return CPU_Rep.f_1_EDX_[11]; }
	static bool CMOV(void) { return CPU_Rep.f_1_EDX_[15]; }
	static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }
	static bool MMX(void) { return CPU_Rep.f_1_EDX_[23]; }
	static bool FXSR(void) { return CPU_Rep.f_1_EDX_[24]; }
	static bool SSE(void) { return CPU_Rep.f_1_EDX_[25]; }
	static bool SSE2(void) { return CPU_Rep.f_1_EDX_[26]; }

	static bool FSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }
	static bool BMI1(void) { return CPU_Rep.f_7_EBX_[3]; }
	static bool HLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
	static bool AVX2(void) { return CPU_Rep.f_7_EBX_[5]; }
	static bool BMI2(void) { return CPU_Rep.f_7_EBX_[8]; }
	static bool ERMS(void) { return CPU_Rep.f_7_EBX_[9]; }
	static bool INVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }
	static bool RTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
	static bool AVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }
	static bool RDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }
	static bool ADX(void) { return CPU_Rep.f_7_EBX_[19]; }
	static bool AVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }
	static bool AVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }
	static bool AVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }
	static bool SHA(void) { return CPU_Rep.f_7_EBX_[29]; }

	static bool PREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }

	static bool LAHF(void) { return CPU_Rep.f_81_ECX_[0]; }
	static bool LZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
	static bool ABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
	static bool SSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
	static bool XOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
	static bool TBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

	static bool SYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
	static bool MMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
	static bool RDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
	static bool _3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
	static bool _3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

	// private:
	static const InstructionSet_Internal CPU_Rep;

	class InstructionSet_Internal
	{
	public:
		InstructionSet_Internal()
			: nIds_{ 0 },
			nExIds_{ 0 },
			isIntel_{ false },
			isAMD_{ false },
			f_1_ECX_{ 0 },
			f_1_EDX_{ 0 },
			f_7_EBX_{ 0 },
			f_7_ECX_{ 0 },
			f_81_ECX_{ 0 },
			f_81_EDX_{ 0 },
			data_{},
			extdata_{}
		{
			//int cpuInfo[4] = {-1};
			std::array<int, 4> cpui;

			// 以0x0作为函数_id参数调用__cpuid获取最高有效函数ID的编号。
			__cpuid(cpui.data(), 0);
			nIds_ = cpui[0];

			for (int i = 0; i <= nIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				data_.push_back(cpui);
			}

			// 捕获供应商字符串
			char vendor[0x20];
			memset(vendor, 0, sizeof(vendor));
			*reinterpret_cast<int*>(vendor) = data_[0][1];
			*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
			*reinterpret_cast<int*>(vendor + 8) = data_[0][2];
			vendor_ = vendor;
			if (vendor_ == "GenuineIntel")
			{
				isIntel_ = true;
			}
			else if (vendor_ == "AuthenticAMD")
			{
				isAMD_ = true;
			}

			// 加载具有函数0x0000001标志的位集
			if (nIds_ >= 1)
			{
				f_1_ECX_ = data_[1][2];
				f_1_EDX_ = data_[1][3];
			}

			// 加载具有函数0x0000007标志的位集
			if (nIds_ >= 7)
			{
				f_7_EBX_ = data_[7][1];
				f_7_ECX_ = data_[7][2];
			}

			// 以0x8000000作为函数_id参数调用__cpuid获取最高有效扩展ID的号码。
			__cpuid(cpui.data(), 0x80000000);
			nExIds_ = cpui[0];

			char brand[0x40];
			memset(brand, 0, sizeof(brand));

			for (int i = 0x80000000; i <= nExIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				extdata_.push_back(cpui);
			}

			// 加载具有函数0x8000001标志的位集
			if (nExIds_ >= 0x80000001)
			{
				f_81_ECX_ = extdata_[1][2];
				f_81_EDX_ = extdata_[1][3];
			}

			// 如果报告，解释中央处理器品牌字符串
			if (nExIds_ >= 0x80000004)
			{
				memcpy(brand, extdata_[2].data(), sizeof(cpui));
				memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
				memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
				brand_ = brand;
			}
		};

		int nIds_;
		int nExIds_;
		std::string vendor_;
		std::string brand_;
		bool isIntel_;
		bool isAMD_;
		std::bitset<32> f_1_ECX_;
		std::bitset<32> f_1_EDX_;
		std::bitset<32> f_7_EBX_;
		std::bitset<32> f_7_ECX_;
		std::bitset<32> f_81_ECX_;
		std::bitset<32> f_81_EDX_;
		std::vector<std::array<int, 4>> data_;
		std::vector<std::array<int, 4>> extdata_;
	};
};

//------------------------------------------------------------------------------
// x86、x64处理器，使用__cpuid来获取有关CPU所支持的扩展指令集的信息
// 初始化静态成员数据
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;
//==============================================================================================

#endif // AVX_SSE_ENABLE


// 在 GetReadableRegions 中添加更严格过滤
bool IsReadable(DWORD protect) {
	return (protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)) &&
		!(protect & (PAGE_GUARD | PAGE_NOACCESS));
}

// 获取进程可读内存区域列表
std::vector<MEMORY_BASIC_INFORMATION> GetReadableRegions() {
	std::vector<MEMORY_BASIC_INFORMATION> regions;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	ULONG_PTR startAddr = (ULONG_PTR)sysInfo.lpMinimumApplicationAddress;
	ULONG_PTR endAddr = (ULONG_PTR)sysInfo.lpMaximumApplicationAddress;

	while (startAddr < endAddr) {
		MEMORY_BASIC_INFORMATION mbi;
		if (VirtualQuery((LPCVOID)startAddr, &mbi, sizeof(mbi))) {
			// 仅扫描可读且已提交的内存
			if (IsReadable(mbi.Protect)) {
				regions.push_back(mbi);
				/*
				if (mbi.State != MEM_FREE)
				{
					if (mbi.State != MEM_RESERVE)
					{
					
					}

				}*/

			}
			startAddr = (ULONG_PTR)mbi.BaseAddress + mbi.RegionSize;
		}
		else {
			break;
		}
	}
	return regions;
}

/****************************************************************************************
 * @brief ▲▲▲BFPatternFind-暴力搜索给定内存范围内的特征码(用于搜索内存区域最末尾的少部分字节序列)-By.Haogl-20240906
 * @param startAddr 需要搜索的起始内存地址
 * @param searchSize 要搜索的内存块的大小,可上负下正（以字节为单位）
 * @param vecPtn 已按特定格式转换后的特征码字节序列
 * @param vecMsk 特征字符串中'??'、'?'的掩码('??'和'?'用1替代，非'?'为0)
 * @param vecIdx 不是'??'的特征码字节在原始特征码字节序列(传入的有'??'的特征码)中的索引下标
 * @return 返回搜索到的内存地址(ULONGLONG类型)
 */
inline ULONGLONG BFPatternFind(const ULONGLONG startAddr, const ULONGLONG searchSize,
	const std::vector<UCHAR>& vecPtn, const std::vector<UCHAR>& vecMsk, const std::vector<ULONG>& vecIdx)
{
	if (searchSize < vecPtn.size()) { return 0; }
	PUCHAR maxAddress = (PUCHAR)(startAddr + searchSize);
	PUCHAR currPattern = (PUCHAR)&vecPtn[0]; // 特征码字节序列的首地址
	UCHAR currEqual;
	register UCHAR currPtnCh;
	PUCHAR currAddress = (PUCHAR)startAddr;

	for (size_t iCh = 0; iCh < vecIdx.size() && (size_t)currAddress <= (size_t)maxAddress; iCh++)
	{
		currPtnCh = currPattern[vecIdx[iCh]];

		currPattern[vecIdx.at(iCh)] = currPtnCh + 0x1;  // 把自己改得不是原来的自己，防止搜索到自己
		// currEqual为0时表示两个字节相同(包含半字节'?'的判断)
		currEqual = ((currAddress[vecIdx[iCh]] | vecMsk.at(vecIdx[iCh])) ^ currPtnCh);
		currPattern[vecIdx.at(iCh)] = currPtnCh; // 过了判断后恢复原来的自己

		if (currEqual) { return 0; } // currEqual不为0时两个字节不相同

		if (iCh + 1 == vecIdx.size()) // 相同数据的个数等于特征码字节数长度，说明找到特征码字节序列
		{
			return (ULONGLONG)currAddress;
		}
	}
	return 0;
}
/****************************************************************************************
* @brief 特征码初始化，将传入的特征码字符串myPattern格式转化为目标特征码字节序列
* @param myPattern 特征码文本字符串
* @param vecPtn 存储转换后的特征码字节序列的向量
* @param vecMsk 存储特征字符串中'??'、'?'的掩码('??'和'?'用1替代，非'?'为0)
* @param vecIdx 存储不是'??'的特征码字节在原始特征码字节序列(传入的有'??'的特征码)中的索引下标
* @return 转换成功返回TRUE，失败返回FALSE
*/
inline BOOL InitPattern(const std::string& myPattern, std::vector<UCHAR>& vecPtn, std::vector<UCHAR>& vecMsk, std::vector<ULONG>& vecIdx)
{
	std::string patternText = myPattern;
	if (patternText.empty()) { return FALSE; }
	// 去除所有空格
	patternText.erase(std::remove(patternText.begin(), patternText.end(), ' '), patternText.end());

	for (char ch : patternText) {
		if (ch != '?' && !((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))) { return FALSE; }
	}

	if (patternText.length() % 2 != 0) { return FALSE; } // 特征码长度不能为单数
	ULONG len = patternText.length() / 2; // 原始特征码长度(字节)

	for (ULONG i = 0; i < len; i++)
	{
		std::string tmpS = patternText.substr(i * 2, 2); // 从patternText的第 j*2 个字符处开始，取2个字符。
		if ("??" != tmpS) // 不是"??"的特征字符
		{
			if ('?' == tmpS.at(0)) // 左半字节为'?'
			{
				tmpS.at(0) = 'F';
				vecMsk.push_back(UCHAR(0xFF) << 4);
			}
			else if ('?' == tmpS.at(1)) // 右半字节为'?'
			{
				tmpS.at(1) = 'F';
				vecMsk.push_back(UCHAR(0xFF) >> 4);
			}
			else
			{
				vecMsk.push_back(UCHAR(0x00)); // 需要做比较的特征字节掩码全为0
			}
			vecIdx.push_back(i);
		}
		if ("??" == tmpS) // 为"??"的特征字符
		{
			tmpS.at(0) = 'F';
			tmpS.at(1) = 'F';
			vecMsk.push_back(UCHAR(0xFF));
		}
		vecPtn.push_back(strtoul(tmpS.c_str(), nullptr, 16));
	}
	if (0 == vecIdx.size()) { return FALSE; }
	return TRUE;
}
/****************************************************************************************
 * @brief ▲▲▲NormalPatternFind-普通暴力搜索内存特征码(支持模糊匹配)
 * @param retList 用于存储搜索到的特征码对应的内存地址（可以存储多个搜索到的内存地址）
 * @param searchStartAddr 需要搜索的起始内存地址
 * @param searchSize 要搜索的内存块的大小,可上负下正（以字节为单位）
 * @param myPattern 搜索特征码，支持通配符??、?，如："?? 5? 77 ?? 88 ?? ?A ??"
 * @param offsetSize 特征码地址离目标地址的偏移距离，上负下正，默认0不偏移
 * @param searchNum 搜索个数，0为搜索整个模块，默认为0
 * @return 成功返回TRUE，失败返回FALSE
*/
BOOL NormalPatternFind(std::vector<ULONGLONG>& retList, const ULONGLONG searchStartAddr,
	const LONGLONG searchSize, const std::string& myPattern,
	const LONGLONG offsetSize, const ULONGLONG searchNum)
{
	if (0 == searchStartAddr || 0 == searchSize) { return FALSE; }

	ULONGLONG realStartAddr = searchStartAddr;
	ULONGLONG absSearchSize = std::abs(searchSize);

	// 处理向上搜索的情况
	if (searchSize < 0) {
		if (searchStartAddr > absSearchSize) {
			realStartAddr = searchStartAddr - absSearchSize;
		}
		else {
			return FALSE; // 无效的搜索范围
		}
	}

	// 解析特征码
	std::vector<UCHAR> vecPtn, vecMsk;
	std::vector<ULONG> vecIdx;
	if (!InitPattern(myPattern, vecPtn, vecMsk, vecIdx)) {
		return FALSE;
	}

	const size_t patternSize = vecPtn.size();
	if (patternSize == 0 || absSearchSize < patternSize) {
		return FALSE;
	}

	// 暴力搜索
	PUCHAR startPtr = (PUCHAR)realStartAddr;
	PUCHAR endPtr = startPtr + absSearchSize - patternSize;

	for (PUCHAR addr = startPtr; addr <= endPtr; addr++) {
		bool match = true;

		// 检查每个字节
		for (size_t i = 0; i < patternSize; i++) {
			UCHAR memByte = addr[i];
			UCHAR patternByte = vecPtn[i];
			UCHAR maskByte = vecMsk[i];

			// 使用掩码处理通配符
			if ((memByte & ~maskByte) != (patternByte & ~maskByte)) {
				match = false;
				break;
			}
		}

		if (match) {
			retList.push_back((ULONGLONG)addr + offsetSize);
			if (searchNum > 0 && retList.size() >= searchNum) {
				return TRUE;
			}
		}
	}

	return TRUE;
}
#ifdef AVX_SSE_ENABLE



/****************************************************************************************
 * @brief ▲▲▲AVX2PatternFind256-使用AVX2加速搜索内存特征码(支持模糊匹配)-By.Haogl-20240906
 * @param retList 用于存储搜索到的特征码对应的内存地址（可以存储多个搜索到的内存地址）
 * @param searchStartAddr 需要搜索的起始内存地址
 * @param searchSize 要搜索的内存块的大小,可上负下正（以字节为单位）
 * @param myPattern 搜索特征码，支持通配符??、?，如："?? 5? 77 ?? 88 ?? ?A ??"
 * @param offsetSize 特征码地址离目标地址的偏移距离，上负下正，默认0不偏移
 * @param searchNum 搜索个数，0为搜索整个模块，默认为0
 * @return 成功返回TRUE，失败返回FALSE
*/
BOOL AVX2PatternFind256(std::vector<ULONGLONG>& retList, const ULONGLONG searchStartAddr,
	const LONGLONG searchSize, const std::string& myPattern, const LONGLONG offsetSize, const ULONGLONG searchNum)
{
	if (0 == searchStartAddr || 0 == searchSize) { return FALSE; }

	ULONGLONG realStartAddr = searchStartAddr; // searchSize为负时(从给定的地址往上搜索)，searchStartAddr需要修正为真正的起始地址
	if ((searchSize < 0) && (searchStartAddr > std::abs(searchSize))) //searchSize可上负下正（以字节为单位）
	{
		realStartAddr = searchStartAddr - std::abs(searchSize);
	}

	std::vector<UCHAR> vecPtn;  // 存储转换后的特征码字节序列
	vecPtn.reserve(16);  // 设置容器的初始预留空间
	std::vector<UCHAR> vecMsk;  // 存储特征字符串中'??'、'?'的掩码('??'和'?'用1替代，非'?'为0)
	vecMsk.reserve(16);
	std::vector<ULONG> vecIdx;  // 存储不是'??'的特征码字节在原始特征码字节序列(传入的有'??'的特征码)中的索引下标
	vecIdx.reserve(8);
	if (!InitPattern(myPattern, vecPtn, vecMsk, vecIdx)) { return FALSE; }

	std::vector<__m256i> m256VecPtn;
	m256VecPtn.reserve(16);
	std::vector<__m256i> m256VecMsk;
	m256VecMsk.reserve(16);
	for (size_t k = 0; k < vecIdx.size(); k++)
	{
		m256VecPtn.push_back(_mm256_set1_epi8(vecPtn.at(vecIdx[k])));
		m256VecMsk.push_back(_mm256_set1_epi8(vecMsk.at(vecIdx[k])));
	}

	UCHAR bakVecPtnCh = vecPtn.at(vecIdx[0]);
	vecPtn.at(vecIdx[0]) += 1;  // 下面搜索前把自己改得不是自己，防止搜索到自己

	retList.clear();
	retList.reserve(16);
	__m256i curMemByte, curCmp, curByteCorr;
	register size_t curBit = 0;
	PUCHAR currMemAddr;
	size_t maxEndSize = min(std::abs(searchSize) - vecPtn.size(), std::abs(searchSize) - 32);
	if (std::abs(searchSize) < 32) { goto lessThan32Byte; }  // 要搜索的内存区域小于32字节
	for (size_t i = vecIdx[0]; i <= maxEndSize; i += 32)
	{
		PUCHAR baseMemAddr = (PUCHAR)(realStartAddr + i - vecIdx[0]);
		size_t prevCmpBit = 0xFFFFFFFF;  // prevCmpBit记录上一个特征字节的bit位比较结果
		for (size_t j = 0; j < vecIdx.size(); j++)
		{
			curMemByte = _mm256_loadu_si256((__m256i*)(baseMemAddr + vecIdx[j]));
			curByteCorr = _mm256_or_si256(curMemByte, m256VecMsk.at(j));        // 当前内存字节的'?'位置的二进制位修正为1
			curCmp = _mm256_cmpeq_epi8(m256VecPtn.at(j), curByteCorr); // 进行字节比较，并将比较结果存储在curCmp中。
			curBit = _mm256_movemask_epi8(curCmp); // 将curCmp中的每个字节的最高位提取出来，并将这些位组合成一个32位的整数curBit。
			curBit = curBit & prevCmpBit;

			if (0 == curBit) { break; }  // 在给定的内存区域中没有找到特征码
			prevCmpBit = curBit;

			if (j + 1 == vecIdx.size()) // 相同数据的个数等于特征码字节数长度，说明找到特征码字节序列
			{
				ULONG bitIdx = 0, n = 0;
				while (_BitScanForward(&bitIdx, curBit))  // 遍历不为0的二进制位(找到的特征码位置)
				{
					currMemAddr = baseMemAddr + n + bitIdx;
					retList.push_back((size_t)(currMemAddr + offsetSize));
					if (searchNum != 0 && retList.size() >= searchNum) { return TRUE; }
					++bitIdx;
					curBit = curBit >> bitIdx;
					n += bitIdx;
				}
			}
		}
	}
	vecPtn.at(vecIdx[0]) = bakVecPtnCh;  // 恢复原来的自己
lessThan32Byte:
	if (vecPtn.size() < 32)  // 给定内存区域末尾搜剩的少于32字节的区域进行搜索
	{
		ULONGLONG tmpStarAddr = realStartAddr + maxEndSize + 1;
		ULONGLONG tmpSearchSize = std::abs(searchSize) - maxEndSize - 1;

		for (int i = 0; i <= tmpSearchSize - vecPtn.size(); i += vecPtn.size())
		{
			ULONGLONG tailPtnAddr = BFPatternFind(tmpStarAddr + i, tmpSearchSize - i, vecPtn, vecMsk, vecIdx);
			if (tailPtnAddr)
			{
				retList.push_back(tailPtnAddr);
			}
		}
	}
	return TRUE;
}
BOOL SSE2PatternFind128(std::vector<ULONGLONG>& retList, const ULONGLONG searchStartAddr,
	const LONGLONG searchSize, const std::string& myPattern, const LONGLONG offsetSize, const ULONGLONG searchNum)
{
	if (0 == searchStartAddr || 0 == searchSize) { return FALSE; }

	ULONGLONG realStartAddr = searchStartAddr; // searchSize为负时(从给定的地址往上搜索)，searchStartAddr需要修正为真正的起始地址
	if ((searchSize < 0) && (searchStartAddr > std::abs(searchSize))) //searchSize可上负下正（以字节为单位）
	{
		realStartAddr = searchStartAddr - std::abs(searchSize);
	}

	std::vector<UCHAR> vecPtn;  // 存储转换后的特征码字节序列
	vecPtn.reserve(16);  // 设置容器的初始预留空间
	std::vector<UCHAR> vecMsk;  // 存储特征字符串中'??'、'?'的掩码('??'和'?'用1替代，非'?'为0)
	vecMsk.reserve(16);
	std::vector<ULONG> vecIdx;  // 存储不是'??'的特征码字节在原始特征码字节序列(传入的有'??'的特征码)中的索引下标
	vecIdx.reserve(8);
	if (!InitPattern(myPattern, vecPtn, vecMsk, vecIdx)) { return FALSE; }

	std::vector<__m128i> m128VecPtn;
	m128VecPtn.reserve(16);
	std::vector<__m128i> m128VecMsk;
	m128VecMsk.reserve(16);
	for (size_t k = 0; k < vecIdx.size(); k++)
	{
		m128VecPtn.push_back(_mm_set1_epi8(vecPtn.at(vecIdx[k])));
		m128VecMsk.push_back(_mm_set1_epi8(vecMsk.at(vecIdx[k])));
	}

	UCHAR bakVecPtnCh = vecPtn.at(vecIdx[0]);
	vecPtn.at(vecIdx[0]) += 1;  // 下面搜索前把自己改得不是自己，防止搜索到自己

	retList.clear();
	retList.reserve(16);
	__m128i curMemByte, curCmp, curByteCorr;
	register size_t curBit = 0;
	PUCHAR currMemAddr;
	size_t maxEndSize = min(std::abs(searchSize) - vecPtn.size(), std::abs(searchSize) - 16);
	if (std::abs(searchSize) < 16) { goto lessThan16Byte; }  // 要搜索的内存区域小于16字节
	for (size_t i = vecIdx[0]; i <= maxEndSize; i += 16)
	{
		PUCHAR baseMemAddr = (PUCHAR)(realStartAddr + i - vecIdx[0]);
		size_t prevCmpBit = 0xFFFFFFFF;
		for (size_t j = 0; j < vecIdx.size(); j++)
		{
			curMemByte = _mm_loadu_si128((__m128i*)(baseMemAddr + vecIdx[j]));
			curByteCorr = _mm_or_si128(curMemByte, m128VecMsk.at(j));	// 当前内存字节的'?'位置的二进制位修正为1
			curCmp = _mm_cmpeq_epi8(m128VecPtn.at(j), curByteCorr); // 进行字节比较，并将比较结果存储在curCmp中。
			curBit = _mm_movemask_epi8(curCmp); // 将curCmp中的每个字节的最高位提取出来，并将这些位组合成一个16位的整数curBit。
			curBit = curBit & prevCmpBit;

			if (0 == curBit) { break; }  // 在给定的内存区域中没有找到特征码
			prevCmpBit = curBit;

			if (j + 1 == vecIdx.size()) // 相同数据的个数等于特征码字节数长度，说明找到特征码字节序列
			{
				ULONG bitIdx = 0, n = 0;
				while (_BitScanForward(&bitIdx, curBit))  // 遍历不为0的二进制位(找到的特征码位置)
				{
					currMemAddr = baseMemAddr + n + bitIdx;
					retList.push_back((ULONGLONG)(currMemAddr + offsetSize));
					if (searchNum != 0 && retList.size() >= searchNum) { return TRUE; }

					++bitIdx;
					curBit = curBit >> bitIdx;
					n += bitIdx;
				}
			}
		}
	}

	vecPtn.at(vecIdx[0]) = bakVecPtnCh;  // 恢复原来的自己

lessThan16Byte:
	if (vecPtn.size() < 16)  // 给定内存区域末尾搜剩的少于16字节的区域进行搜索
	{
		ULONGLONG tmpStarAddr = realStartAddr + maxEndSize + 1;
		ULONGLONG tmpSearchSize = std::abs(searchSize) - maxEndSize - 1;

		for (int i = 0; i <= tmpSearchSize - vecPtn.size(); i += vecPtn.size())
		{
			ULONGLONG tailPtnAddr = BFPatternFind(tmpStarAddr + i, tmpSearchSize - i, vecPtn, vecMsk, vecIdx);
			if (tailPtnAddr)
			{
				retList.push_back(tailPtnAddr);
			}
		}
	}
	return TRUE;
}
#endif // AVX_SSE_ENABLE


/****************************************************************************************
 * @brief ▲▲▲HglPatternFindEx-判断CPU支持的指令集，自动选择使用SSE2/AVX2加速搜索内存特征码(支持模糊匹配)-By.Haogl-20240906
 * @param retList 用于存储搜索到的特征码对应的内存地址（可以存储多个搜索到的内存地址）
 * @param searchStartAddr 需要搜索的起始内存地址
 * @param searchSize 要搜索的内存块的大小,可上负下正（以字节为单位）
 * @param myPattern 搜索特征码，支持通配符??、?，如："?? 5? 77 ?? 88 ?? ?A ??"
 * @param offsetSize 特征码地址离目标地址的偏移距离，上负下正，默认0不偏移
 * @param searchNum 搜索个数，0为搜索整个模块，默认为0
 * @return 成功返回TRUE，失败返回FALSE
*/
BOOL HglPatternFindEx(std::vector<ULONGLONG>& retList, const ULONGLONG searchStartAddr,
	const LONGLONG searchSize, const std::string& myPattern, const LONGLONG offsetSize, const ULONGLONG searchNum)
{

	bool ret = false;

#ifdef AVX_SSE_ENABLE
	InstructionSet instrSet;
	if ((bool)instrSet.AVX2)
	{
		ret = AVX2PatternFind256(retList, searchStartAddr, searchSize, myPattern, offsetSize, searchNum);
	}
	else if ((bool)instrSet.SSE2)
	{
		ret = SSE2PatternFind128(retList, searchStartAddr, searchSize, myPattern, offsetSize, searchNum);
	}
	else
	

#endif // AVX_SSE_ENABLE

	{
		ret = NormalPatternFind(retList, searchStartAddr, searchSize, myPattern, offsetSize, searchNum);
	}
	

	//ret = NormalPatternFind(retList, searchStartAddr, searchSize, myPattern, offsetSize, searchNum);
	//return ret;

	
	return ret;
}
// 获取模块基址和大小（适用于当前进程）
MODULEINFO MemUnits::GetCurrentModuleInfo(const char* moduleName) {
	HMODULE hModule = GetModuleHandleA(moduleName);
	MODULEINFO modInfo = { 0 };
	if (hModule) {
		GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo));
	}
	return modInfo;
}

#include <psapi.h>
// 获取模块实际占用内存区域大小
ULONG_PTR GetModuleMemorySize(MODULEINFO modInfo) {
	MEMORY_BASIC_INFORMATION mbi;
	ULONG_PTR totalSize = 0;
	LPVOID baseAddress = modInfo.lpBaseOfDll;
	ULONG_PTR currentAddr = (ULONG_PTR)baseAddress;

	//MODULEINFO modInfo= GetCurrentModuleInfo("Revc.exe");

	while (VirtualQuery((LPCVOID)currentAddr, &mbi, sizeof(mbi))) {
		// 属于模块的已提交内存区域
		if (mbi.AllocationBase == baseAddress &&
			mbi.State == MEM_COMMIT &&
			(mbi.Type == MEM_IMAGE || mbi.Type == MEM_PRIVATE)) {
			totalSize += mbi.RegionSize;
		}
		currentAddr += mbi.RegionSize;

		// 超出模块范围时终止
		if (currentAddr >= (ULONG_PTR)baseAddress + (&modInfo)->SizeOfImage)
			break;
	}
	return totalSize;
}
std::vector<ULONGLONG> MemUnits::ScanMemory(const std::string& pattern, const char* ModelName) {
	std::vector<ULONGLONG> results;

	// 获取可读内存区域
	auto regions = GetReadableRegions();

	// 初始化特征码参数
	std::vector<UCHAR> vecPtn, vecMsk;
	std::vector<ULONG> vecIdx;
	if (!InitPattern(pattern, vecPtn, vecMsk, vecIdx)) {
		return results;
	}
	MODULEINFO modInfo = GetCurrentModuleInfo(ModelName);
	ULONG_PTR actualSize = GetModuleMemorySize(modInfo);

	// 使用AVX2加速搜索
	//std::vector<ULONGLONG> regionResults;
	//HglPatternFindEx(regionResults, (ULONGLONG)modInfo.lpBaseOfDll, modInfo.SizeOfImage, pattern, 0, 0);

	for (const auto& mbi : regions) {
		ULONGLONG startAddr = (ULONGLONG)mbi.BaseAddress; 
		ULONGLONG regionSize = (ULONGLONG)mbi.RegionSize;
		ULONGLONG moduleStart = (ULONGLONG)modInfo.lpBaseOfDll;
		ULONGLONG moduleEnd = moduleStart + modInfo.SizeOfImage;
		// 跳过不在模块范围内的区域
		if (startAddr >= moduleEnd) break;
		if (startAddr + regionSize <= moduleStart) continue;

		// 裁剪到模块范围
		ULONGLONG scanStart = max(startAddr, moduleStart);
		ULONGLONG scanEnd = min(startAddr + regionSize, moduleEnd);
		ULONGLONG scanSize = scanEnd > scanStart ? scanEnd - scanStart : 0;
		if (scanSize == 0) continue;

		std::vector<ULONGLONG> regionResults;
		HglPatternFindEx(regionResults, scanStart, scanSize, pattern, 0, 0);

		results.insert(results.end(), regionResults.begin(), regionResults.end());
	}

	return results;
}


#define  RESERVED_ADDRESS_X32  0x00327000
#define  SYSTEM_MEMORY_ADDRESS_X32 0x70000000
#define  SYSTEM_MEMORY_ADDRESS_X32_MAX 0x7FFFFFFF

/*
// 定义区段上限,一般的区段大小不会超过
#define SECTION_SIZE_X32_MAX  0x0f000000
char* getProcessName() {
	char processFullName[MAX_PATH] = { 0 };
	char processName[MAX_PATH] = { 0 };

	// 获取当前进程ID
	DWORD processId = GetCurrentProcessId();

	// 获取进程完整路径
	GetModuleFileNameA(NULL, processFullName, MAX_PATH);

	// 提取进程名
	char* tmp1 = strrchr(processFullName, '\\');
	//char* tmp2 = strrchr(processFullName, '.');
	strcpy_s(processName, MAX_PATH, tmp1 ? tmp1 + 1 : processFullName);
	//memcpy(processName, tmp1 + 1, min(tmp1 - 1, MAX_PATH));


	return processName;
}*/
/*
std::vector <DWORD> FindSigX32(DWORD dwPid, const char* Value, ULONG Start, ULONG End)
{
	std::vector <DWORD> vFindResult;
	vector<ULONG64>  dwValue;
	//bool b = AVX2PatternFind256(dwValue, Start, 2048, Value, 0, 0);
	//dwValue = ScanEntireProcessMemory(Value, "Revc.exe");
	dwValue = ScanEntireMemory(Value, getProcessName());
	if (dwValue.size() <= 0)
	{
		return vFindResult;
	}
	for (size_t i = 0; i < dwValue.size(); i++)
	{
		vFindResult.push_back(dwValue[i]);
	}
	return vFindResult;


}*/

