#include <MemUtils.h>

#include "pch.h"
#include <iostream>

#include <mutex>
#include <bitset>
#include <array> // Ensure this header is included for std::array

using namespace std;
//ָ�����
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

#include <ppl.h>  // ����ģʽ��
#include <emmintrin.h> // ��Ӵ�ͷ�ļ��Խ��δ�����ʶ�� "__m128i" �� "_mm_loadu_si128" ������

//= ============================================================================================ =
//==============================================================================================
// x86��x64��������ʹ��__cpuid����ȡ�й�CPU��֧�ֵ���չָ�����Ϣ
class InstructionSet
{
	class InstructionSet_Internal;  // ǰ������

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

			// ��0x0��Ϊ����_id��������__cpuid��ȡ�����Ч����ID�ı�š�
			__cpuid(cpui.data(), 0);
			nIds_ = cpui[0];

			for (int i = 0; i <= nIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				data_.push_back(cpui);
			}

			// ����Ӧ���ַ���
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

			// ���ؾ��к���0x0000001��־��λ��
			if (nIds_ >= 1)
			{
				f_1_ECX_ = data_[1][2];
				f_1_EDX_ = data_[1][3];
			}

			// ���ؾ��к���0x0000007��־��λ��
			if (nIds_ >= 7)
			{
				f_7_EBX_ = data_[7][1];
				f_7_ECX_ = data_[7][2];
			}

			// ��0x8000000��Ϊ����_id��������__cpuid��ȡ�����Ч��չID�ĺ��롣
			__cpuid(cpui.data(), 0x80000000);
			nExIds_ = cpui[0];

			char brand[0x40];
			memset(brand, 0, sizeof(brand));

			for (int i = 0x80000000; i <= nExIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				extdata_.push_back(cpui);
			}

			// ���ؾ��к���0x8000001��־��λ��
			if (nExIds_ >= 0x80000001)
			{
				f_81_ECX_ = extdata_[1][2];
				f_81_EDX_ = extdata_[1][3];
			}

			// ������棬�������봦����Ʒ���ַ���
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
// x86��x64��������ʹ��__cpuid����ȡ�й�CPU��֧�ֵ���չָ�����Ϣ
// ��ʼ����̬��Ա����
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;
//==============================================================================================

#endif // AVX_SSE_ENABLE


// �� GetReadableRegions ����Ӹ��ϸ����
bool IsReadable(DWORD protect) {
	return (protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)) &&
		!(protect & (PAGE_GUARD | PAGE_NOACCESS));
}

// ��ȡ���̿ɶ��ڴ������б�
std::vector<MEMORY_BASIC_INFORMATION> GetReadableRegions() {
	std::vector<MEMORY_BASIC_INFORMATION> regions;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	ULONG_PTR startAddr = (ULONG_PTR)sysInfo.lpMinimumApplicationAddress;
	ULONG_PTR endAddr = (ULONG_PTR)sysInfo.lpMaximumApplicationAddress;

	while (startAddr < endAddr) {
		MEMORY_BASIC_INFORMATION mbi;
		if (VirtualQuery((LPCVOID)startAddr, &mbi, sizeof(mbi))) {
			// ��ɨ��ɶ������ύ���ڴ�
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
 * @brief ������BFPatternFind-�������������ڴ淶Χ�ڵ�������(���������ڴ�������ĩβ���ٲ����ֽ�����)-By.Haogl-20240906
 * @param startAddr ��Ҫ��������ʼ�ڴ��ַ
 * @param searchSize Ҫ�������ڴ��Ĵ�С,���ϸ����������ֽ�Ϊ��λ��
 * @param vecPtn �Ѱ��ض���ʽת������������ֽ�����
 * @param vecMsk �����ַ�����'??'��'?'������('??'��'?'��1�������'?'Ϊ0)
 * @param vecIdx ����'??'���������ֽ���ԭʼ�������ֽ�����(�������'??'��������)�е������±�
 * @return �������������ڴ��ַ(ULONGLONG����)
 */
inline ULONGLONG BFPatternFind(const ULONGLONG startAddr, const ULONGLONG searchSize,
	const std::vector<UCHAR>& vecPtn, const std::vector<UCHAR>& vecMsk, const std::vector<ULONG>& vecIdx)
{
	if (searchSize < vecPtn.size()) { return 0; }
	PUCHAR maxAddress = (PUCHAR)(startAddr + searchSize);
	PUCHAR currPattern = (PUCHAR)&vecPtn[0]; // �������ֽ����е��׵�ַ
	UCHAR currEqual;
	register UCHAR currPtnCh;
	PUCHAR currAddress = (PUCHAR)startAddr;

	for (size_t iCh = 0; iCh < vecIdx.size() && (size_t)currAddress <= (size_t)maxAddress; iCh++)
	{
		currPtnCh = currPattern[vecIdx[iCh]];

		currPattern[vecIdx.at(iCh)] = currPtnCh + 0x1;  // ���Լ��ĵò���ԭ�����Լ�����ֹ�������Լ�
		// currEqualΪ0ʱ��ʾ�����ֽ���ͬ(�������ֽ�'?'���ж�)
		currEqual = ((currAddress[vecIdx[iCh]] | vecMsk.at(vecIdx[iCh])) ^ currPtnCh);
		currPattern[vecIdx.at(iCh)] = currPtnCh; // �����жϺ�ָ�ԭ�����Լ�

		if (currEqual) { return 0; } // currEqual��Ϊ0ʱ�����ֽڲ���ͬ

		if (iCh + 1 == vecIdx.size()) // ��ͬ���ݵĸ��������������ֽ������ȣ�˵���ҵ��������ֽ�����
		{
			return (ULONGLONG)currAddress;
		}
	}
	return 0;
}
/****************************************************************************************
* @brief �������ʼ������������������ַ���myPattern��ʽת��ΪĿ���������ֽ�����
* @param myPattern �������ı��ַ���
* @param vecPtn �洢ת������������ֽ����е�����
* @param vecMsk �洢�����ַ�����'??'��'?'������('??'��'?'��1�������'?'Ϊ0)
* @param vecIdx �洢����'??'���������ֽ���ԭʼ�������ֽ�����(�������'??'��������)�е������±�
* @return ת���ɹ�����TRUE��ʧ�ܷ���FALSE
*/
inline BOOL InitPattern(const std::string& myPattern, std::vector<UCHAR>& vecPtn, std::vector<UCHAR>& vecMsk, std::vector<ULONG>& vecIdx)
{
	std::string patternText = myPattern;
	if (patternText.empty()) { return FALSE; }
	// ȥ�����пո�
	patternText.erase(std::remove(patternText.begin(), patternText.end(), ' '), patternText.end());

	for (char ch : patternText) {
		if (ch != '?' && !((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))) { return FALSE; }
	}

	if (patternText.length() % 2 != 0) { return FALSE; } // �����볤�Ȳ���Ϊ����
	ULONG len = patternText.length() / 2; // ԭʼ�����볤��(�ֽ�)

	for (ULONG i = 0; i < len; i++)
	{
		std::string tmpS = patternText.substr(i * 2, 2); // ��patternText�ĵ� j*2 ���ַ�����ʼ��ȡ2���ַ���
		if ("??" != tmpS) // ����"??"�������ַ�
		{
			if ('?' == tmpS.at(0)) // ����ֽ�Ϊ'?'
			{
				tmpS.at(0) = 'F';
				vecMsk.push_back(UCHAR(0xFF) << 4);
			}
			else if ('?' == tmpS.at(1)) // �Ұ��ֽ�Ϊ'?'
			{
				tmpS.at(1) = 'F';
				vecMsk.push_back(UCHAR(0xFF) >> 4);
			}
			else
			{
				vecMsk.push_back(UCHAR(0x00)); // ��Ҫ���Ƚϵ������ֽ�����ȫΪ0
			}
			vecIdx.push_back(i);
		}
		if ("??" == tmpS) // Ϊ"??"�������ַ�
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
 * @brief ������NormalPatternFind-��ͨ���������ڴ�������(֧��ģ��ƥ��)
 * @param retList ���ڴ洢���������������Ӧ���ڴ��ַ�����Դ洢������������ڴ��ַ��
 * @param searchStartAddr ��Ҫ��������ʼ�ڴ��ַ
 * @param searchSize Ҫ�������ڴ��Ĵ�С,���ϸ����������ֽ�Ϊ��λ��
 * @param myPattern ���������룬֧��ͨ���??��?���磺"?? 5? 77 ?? 88 ?? ?A ??"
 * @param offsetSize �������ַ��Ŀ���ַ��ƫ�ƾ��룬�ϸ�������Ĭ��0��ƫ��
 * @param searchNum ����������0Ϊ��������ģ�飬Ĭ��Ϊ0
 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL NormalPatternFind(std::vector<ULONGLONG>& retList, const ULONGLONG searchStartAddr,
	const LONGLONG searchSize, const std::string& myPattern,
	const LONGLONG offsetSize, const ULONGLONG searchNum)
{
	if (0 == searchStartAddr || 0 == searchSize) { return FALSE; }

	ULONGLONG realStartAddr = searchStartAddr;
	ULONGLONG absSearchSize = std::abs(searchSize);

	// �����������������
	if (searchSize < 0) {
		if (searchStartAddr > absSearchSize) {
			realStartAddr = searchStartAddr - absSearchSize;
		}
		else {
			return FALSE; // ��Ч��������Χ
		}
	}

	// ����������
	std::vector<UCHAR> vecPtn, vecMsk;
	std::vector<ULONG> vecIdx;
	if (!InitPattern(myPattern, vecPtn, vecMsk, vecIdx)) {
		return FALSE;
	}

	const size_t patternSize = vecPtn.size();
	if (patternSize == 0 || absSearchSize < patternSize) {
		return FALSE;
	}

	// ��������
	PUCHAR startPtr = (PUCHAR)realStartAddr;
	PUCHAR endPtr = startPtr + absSearchSize - patternSize;

	for (PUCHAR addr = startPtr; addr <= endPtr; addr++) {
		bool match = true;

		// ���ÿ���ֽ�
		for (size_t i = 0; i < patternSize; i++) {
			UCHAR memByte = addr[i];
			UCHAR patternByte = vecPtn[i];
			UCHAR maskByte = vecMsk[i];

			// ʹ�����봦��ͨ���
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
 * @brief ������AVX2PatternFind256-ʹ��AVX2���������ڴ�������(֧��ģ��ƥ��)-By.Haogl-20240906
 * @param retList ���ڴ洢���������������Ӧ���ڴ��ַ�����Դ洢������������ڴ��ַ��
 * @param searchStartAddr ��Ҫ��������ʼ�ڴ��ַ
 * @param searchSize Ҫ�������ڴ��Ĵ�С,���ϸ����������ֽ�Ϊ��λ��
 * @param myPattern ���������룬֧��ͨ���??��?���磺"?? 5? 77 ?? 88 ?? ?A ??"
 * @param offsetSize �������ַ��Ŀ���ַ��ƫ�ƾ��룬�ϸ�������Ĭ��0��ƫ��
 * @param searchNum ����������0Ϊ��������ģ�飬Ĭ��Ϊ0
 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL AVX2PatternFind256(std::vector<ULONGLONG>& retList, const ULONGLONG searchStartAddr,
	const LONGLONG searchSize, const std::string& myPattern, const LONGLONG offsetSize, const ULONGLONG searchNum)
{
	if (0 == searchStartAddr || 0 == searchSize) { return FALSE; }

	ULONGLONG realStartAddr = searchStartAddr; // searchSizeΪ��ʱ(�Ӹ����ĵ�ַ��������)��searchStartAddr��Ҫ����Ϊ��������ʼ��ַ
	if ((searchSize < 0) && (searchStartAddr > std::abs(searchSize))) //searchSize���ϸ����������ֽ�Ϊ��λ��
	{
		realStartAddr = searchStartAddr - std::abs(searchSize);
	}

	std::vector<UCHAR> vecPtn;  // �洢ת������������ֽ�����
	vecPtn.reserve(16);  // ���������ĳ�ʼԤ���ռ�
	std::vector<UCHAR> vecMsk;  // �洢�����ַ�����'??'��'?'������('??'��'?'��1�������'?'Ϊ0)
	vecMsk.reserve(16);
	std::vector<ULONG> vecIdx;  // �洢����'??'���������ֽ���ԭʼ�������ֽ�����(�������'??'��������)�е������±�
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
	vecPtn.at(vecIdx[0]) += 1;  // ��������ǰ���Լ��ĵò����Լ�����ֹ�������Լ�

	retList.clear();
	retList.reserve(16);
	__m256i curMemByte, curCmp, curByteCorr;
	register size_t curBit = 0;
	PUCHAR currMemAddr;
	size_t maxEndSize = min(std::abs(searchSize) - vecPtn.size(), std::abs(searchSize) - 32);
	if (std::abs(searchSize) < 32) { goto lessThan32Byte; }  // Ҫ�������ڴ�����С��32�ֽ�
	for (size_t i = vecIdx[0]; i <= maxEndSize; i += 32)
	{
		PUCHAR baseMemAddr = (PUCHAR)(realStartAddr + i - vecIdx[0]);
		size_t prevCmpBit = 0xFFFFFFFF;  // prevCmpBit��¼��һ�������ֽڵ�bitλ�ȽϽ��
		for (size_t j = 0; j < vecIdx.size(); j++)
		{
			curMemByte = _mm256_loadu_si256((__m256i*)(baseMemAddr + vecIdx[j]));
			curByteCorr = _mm256_or_si256(curMemByte, m256VecMsk.at(j));        // ��ǰ�ڴ��ֽڵ�'?'λ�õĶ�����λ����Ϊ1
			curCmp = _mm256_cmpeq_epi8(m256VecPtn.at(j), curByteCorr); // �����ֽڱȽϣ������ȽϽ���洢��curCmp�С�
			curBit = _mm256_movemask_epi8(curCmp); // ��curCmp�е�ÿ���ֽڵ����λ��ȡ������������Щλ��ϳ�һ��32λ������curBit��
			curBit = curBit & prevCmpBit;

			if (0 == curBit) { break; }  // �ڸ������ڴ�������û���ҵ�������
			prevCmpBit = curBit;

			if (j + 1 == vecIdx.size()) // ��ͬ���ݵĸ��������������ֽ������ȣ�˵���ҵ��������ֽ�����
			{
				ULONG bitIdx = 0, n = 0;
				while (_BitScanForward(&bitIdx, curBit))  // ������Ϊ0�Ķ�����λ(�ҵ���������λ��)
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
	vecPtn.at(vecIdx[0]) = bakVecPtnCh;  // �ָ�ԭ�����Լ�
lessThan32Byte:
	if (vecPtn.size() < 32)  // �����ڴ�����ĩβ��ʣ������32�ֽڵ������������
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

	ULONGLONG realStartAddr = searchStartAddr; // searchSizeΪ��ʱ(�Ӹ����ĵ�ַ��������)��searchStartAddr��Ҫ����Ϊ��������ʼ��ַ
	if ((searchSize < 0) && (searchStartAddr > std::abs(searchSize))) //searchSize���ϸ����������ֽ�Ϊ��λ��
	{
		realStartAddr = searchStartAddr - std::abs(searchSize);
	}

	std::vector<UCHAR> vecPtn;  // �洢ת������������ֽ�����
	vecPtn.reserve(16);  // ���������ĳ�ʼԤ���ռ�
	std::vector<UCHAR> vecMsk;  // �洢�����ַ�����'??'��'?'������('??'��'?'��1�������'?'Ϊ0)
	vecMsk.reserve(16);
	std::vector<ULONG> vecIdx;  // �洢����'??'���������ֽ���ԭʼ�������ֽ�����(�������'??'��������)�е������±�
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
	vecPtn.at(vecIdx[0]) += 1;  // ��������ǰ���Լ��ĵò����Լ�����ֹ�������Լ�

	retList.clear();
	retList.reserve(16);
	__m128i curMemByte, curCmp, curByteCorr;
	register size_t curBit = 0;
	PUCHAR currMemAddr;
	size_t maxEndSize = min(std::abs(searchSize) - vecPtn.size(), std::abs(searchSize) - 16);
	if (std::abs(searchSize) < 16) { goto lessThan16Byte; }  // Ҫ�������ڴ�����С��16�ֽ�
	for (size_t i = vecIdx[0]; i <= maxEndSize; i += 16)
	{
		PUCHAR baseMemAddr = (PUCHAR)(realStartAddr + i - vecIdx[0]);
		size_t prevCmpBit = 0xFFFFFFFF;
		for (size_t j = 0; j < vecIdx.size(); j++)
		{
			curMemByte = _mm_loadu_si128((__m128i*)(baseMemAddr + vecIdx[j]));
			curByteCorr = _mm_or_si128(curMemByte, m128VecMsk.at(j));	// ��ǰ�ڴ��ֽڵ�'?'λ�õĶ�����λ����Ϊ1
			curCmp = _mm_cmpeq_epi8(m128VecPtn.at(j), curByteCorr); // �����ֽڱȽϣ������ȽϽ���洢��curCmp�С�
			curBit = _mm_movemask_epi8(curCmp); // ��curCmp�е�ÿ���ֽڵ����λ��ȡ������������Щλ��ϳ�һ��16λ������curBit��
			curBit = curBit & prevCmpBit;

			if (0 == curBit) { break; }  // �ڸ������ڴ�������û���ҵ�������
			prevCmpBit = curBit;

			if (j + 1 == vecIdx.size()) // ��ͬ���ݵĸ��������������ֽ������ȣ�˵���ҵ��������ֽ�����
			{
				ULONG bitIdx = 0, n = 0;
				while (_BitScanForward(&bitIdx, curBit))  // ������Ϊ0�Ķ�����λ(�ҵ���������λ��)
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

	vecPtn.at(vecIdx[0]) = bakVecPtnCh;  // �ָ�ԭ�����Լ�

lessThan16Byte:
	if (vecPtn.size() < 16)  // �����ڴ�����ĩβ��ʣ������16�ֽڵ������������
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
 * @brief ������HglPatternFindEx-�ж�CPU֧�ֵ�ָ����Զ�ѡ��ʹ��SSE2/AVX2���������ڴ�������(֧��ģ��ƥ��)-By.Haogl-20240906
 * @param retList ���ڴ洢���������������Ӧ���ڴ��ַ�����Դ洢������������ڴ��ַ��
 * @param searchStartAddr ��Ҫ��������ʼ�ڴ��ַ
 * @param searchSize Ҫ�������ڴ��Ĵ�С,���ϸ����������ֽ�Ϊ��λ��
 * @param myPattern ���������룬֧��ͨ���??��?���磺"?? 5? 77 ?? 88 ?? ?A ??"
 * @param offsetSize �������ַ��Ŀ���ַ��ƫ�ƾ��룬�ϸ�������Ĭ��0��ƫ��
 * @param searchNum ����������0Ϊ��������ģ�飬Ĭ��Ϊ0
 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
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
// ��ȡģ���ַ�ʹ�С�������ڵ�ǰ���̣�
MODULEINFO MemUnits::GetCurrentModuleInfo(const char* moduleName) {
	HMODULE hModule = GetModuleHandleA(moduleName);
	MODULEINFO modInfo = { 0 };
	if (hModule) {
		GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo));
	}
	return modInfo;
}

#include <psapi.h>
// ��ȡģ��ʵ��ռ���ڴ������С
ULONG_PTR GetModuleMemorySize(MODULEINFO modInfo) {
	MEMORY_BASIC_INFORMATION mbi;
	ULONG_PTR totalSize = 0;
	LPVOID baseAddress = modInfo.lpBaseOfDll;
	ULONG_PTR currentAddr = (ULONG_PTR)baseAddress;

	//MODULEINFO modInfo= GetCurrentModuleInfo("Revc.exe");

	while (VirtualQuery((LPCVOID)currentAddr, &mbi, sizeof(mbi))) {
		// ����ģ������ύ�ڴ�����
		if (mbi.AllocationBase == baseAddress &&
			mbi.State == MEM_COMMIT &&
			(mbi.Type == MEM_IMAGE || mbi.Type == MEM_PRIVATE)) {
			totalSize += mbi.RegionSize;
		}
		currentAddr += mbi.RegionSize;

		// ����ģ�鷶Χʱ��ֹ
		if (currentAddr >= (ULONG_PTR)baseAddress + (&modInfo)->SizeOfImage)
			break;
	}
	return totalSize;
}
std::vector<ULONGLONG> MemUnits::ScanMemory(const std::string& pattern, const char* ModelName) {
	std::vector<ULONGLONG> results;

	// ��ȡ�ɶ��ڴ�����
	auto regions = GetReadableRegions();

	// ��ʼ�����������
	std::vector<UCHAR> vecPtn, vecMsk;
	std::vector<ULONG> vecIdx;
	if (!InitPattern(pattern, vecPtn, vecMsk, vecIdx)) {
		return results;
	}
	MODULEINFO modInfo = GetCurrentModuleInfo(ModelName);
	ULONG_PTR actualSize = GetModuleMemorySize(modInfo);

	// ʹ��AVX2��������
	//std::vector<ULONGLONG> regionResults;
	//HglPatternFindEx(regionResults, (ULONGLONG)modInfo.lpBaseOfDll, modInfo.SizeOfImage, pattern, 0, 0);

	for (const auto& mbi : regions) {
		ULONGLONG startAddr = (ULONGLONG)mbi.BaseAddress; 
		ULONGLONG regionSize = (ULONGLONG)mbi.RegionSize;
		ULONGLONG moduleStart = (ULONGLONG)modInfo.lpBaseOfDll;
		ULONGLONG moduleEnd = moduleStart + modInfo.SizeOfImage;
		// ��������ģ�鷶Χ�ڵ�����
		if (startAddr >= moduleEnd) break;
		if (startAddr + regionSize <= moduleStart) continue;

		// �ü���ģ�鷶Χ
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
// ������������,һ������δ�С���ᳬ��
#define SECTION_SIZE_X32_MAX  0x0f000000
char* getProcessName() {
	char processFullName[MAX_PATH] = { 0 };
	char processName[MAX_PATH] = { 0 };

	// ��ȡ��ǰ����ID
	DWORD processId = GetCurrentProcessId();

	// ��ȡ��������·��
	GetModuleFileNameA(NULL, processFullName, MAX_PATH);

	// ��ȡ������
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

