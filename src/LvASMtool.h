
#ifndef LVASMTOOL_H_
#define LVASMTOOL_H_

#include <fstream>
#include <string>
#include <iostream>
#include <map>

#include "Rom.h"
#include "Xkas.h"

using namespace std;

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;

// ラベルの数
const int LABEL_NUM = 12;

class LvASMtool {
public:
	// newが予約語だったの忘れてた
	// アンダーバーつけて誤魔化す
	enum ELvASMver {
		_nothing,	// LevelASM未挿入
		_now,		// rom側とツール側が同じバージョン
		_old,		// rom側の方が古い
		_new,		// rom側の方が新しい
		_edit1754,	// edit1754氏のLevelASMが挿入済み
	};
	LvASMtool();
	LvASMtool(string romName,string listName,int infoLevel);
	~LvASMtool();

	// LevelASMのバージョンを確認
	ELvASMver checkLvASMver();

	// LevelASM MainCodeを挿入
	int insertLevelASMcode();
	int insertLevelASMcode(int insAddr);

	// LevelASMを挿入
	int insertLevelASM();

	// LevelASM MainCodeを削除
	int deleteLevelASMcode();

	// 挿入済みLevelASMを削除
	int deleteLevelASM();

	// romファイルに編集したデータを書き込み
	void writeRomFile();

	// 挿入したコードを戻す
	void rewindHijackCode();

	// edit1754氏のLevelASMを削除
	int deleteEdit1754LevelASM();

	ushort getRomLvASMver();

	// 定数のアクセッサを設けるのってどうなの？
	ushort getToolLvASMver();

	// 未定義ラベルを使用済みか確認
	bool checkUsingUndefLabel();

	// 使用済みの未定義ラベルを一覧
	void showUnsingUndefLabel();

private:
	// リストファイル用構造体
	struct listline {
		int insnum;			// 挿入するLevel番号
		string filename;	// 挿入するasmファイルの名前(拡張子あり)
	};


	// 挿入したアドレス 負数格納したいからSignedで定義〜
	struct addr {
		int base;
		int RTLaddr;
		int labelData[LABEL_NUM];
	};

	string romName = "";
	string listName = "";

	ifstream listFile;
	Rom romFile;

	int romSize = 0;
	int insAddr = -1;
	int insEndAddr = 0;
	ushort lvASMver = 0;

	int infoLevel = 0;
	int headerSize = 0;

	// Header Loromのみ対応
	int convPCtoSNES(int addr);
	int convSNEStoPC(int addr);

	// LeveASM 実行コードのバージョンコード
	// ツール本体のバージョンとは別
	const ushort LEVELASM_CODE_VERSION = 0x0120;

	// LevelASM 実行コードの先頭に挿入する情報
	// "LevelASM tool"の有無で導入済みか判定
	const char *LEVELASM_HEADER =
	//_______0123456789ABCDEF
			"LevelASM tool   "	// 0x00
			"Version:1.20    "	// 0x10
			"Date:2015/01/16 "	// 0x20
			"Author:88-CHAN  "	// 0x30
			"                "	// 0x40
			"                "	// 0x50
			"                ";	// 0x60

	// これ未満のアドレスにはデータを挿入しない
	const int BASE_ADDR = 0x080000;

	// ラベル名
	const char LABEL_NAME[LABEL_NUM][0x10] = {
		"INIT_LEVEL",
		"INIT_EARLY",
		"INIT_LATER",
		"MAIN_EARLY",
		"MAIN_LATER",
		"NMI_LEVEL",
		"RESERVED_1",
		"RESERVED_2",
		"USER_DEF_1",
		"USER_DEF_2",
		"USER_DEF_3",
		"USER_DEF_4",
	};

	// ラベルの別名 ラベル名が見つからなかった時に確認する
	const char LABEL_ALIAS[LABEL_NUM][0x10] = {
			"INIT_LEVEL",
			"INIT_EARLY",
			"INIT",
			"MAIN",
			"MAIN_LATER",
			"NMI",
			"RESV1",
			"RESV2",
			"USER1",
			"USER2",
			"USER3",
			"USER4",
	};

	// ラベルの表示名
	const char OUTPUT_LABEL_NAME[LABEL_NUM][0x10] = {
			"INIT_LEVEL",
			"INIT_EARLY",
			"INIT_LATER",
			"MAIN_EARLY",
			"MAIN_LATER",
			"NMI_LEVEL ",
			"RESERVED_1",
			"RESERVED_2",
			"USER_DEF_1",
			"USER_DEF_2",
			"USER_DEF_3",
			"USER_DEF_4",
	};

	void writeLongAddr(uchar* data,int offset,int addr);
	void writeWordAddr(uchar* data,int offset,int addr);
	listline analyzeListLine(string listLine);
};

#endif /* LVASMTOOL_H_ */
