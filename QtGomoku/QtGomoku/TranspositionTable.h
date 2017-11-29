#pragma once
#include "StaticDefinition.h"

/*
 * 置换表
 * 用来在α-β搜索过程中对状态进行压缩
 * 压缩后的状态以一个散列值的形式存在,便于在以后的搜索中能够快速发现这个状态是否曾经被评价过
 * 如果发现是以前评价过的方法,则直接取用原来的评价值
 */
class TranspositionTable
{
public:
	TranspositionTable();
	~TranspositionTable();
private:
	unsigned int m_nHashKey32[15][10][9];  //32位随机树组，用以生成32位哈希值
	unsigned int m_HashKey32;			   //32位哈希值
	unsigned __int64 m_ulHashKey64[15][10][9];//64位随机树组，用以生成64位哈希值
	__int64 m_HashKey64;          //64 位哈希值
	HashItem *m_pTT[10];          //置换表头指针
private:
	__int64 rand64();
	long rand32();
public:
	void InitializeHashKey();
	void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo);
	int LookUpHashTable(int alpha, int beta, int depth, int TableNo);
	void Hash_UnMakeMove(STONEMOVE *move, unsigned char CurPosition[][GRID_NUM]);
	void Hash_MakeMove(STONEMOVE *move, unsigned char CurPosition[][GRID_NUM]);
	void CalculateInitHashKey(unsigned char CurPosition[][GRID_NUM]);
	void _CTranspositionTable();
};