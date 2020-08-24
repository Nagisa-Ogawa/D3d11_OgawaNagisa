//=============================================================================
// stdafx.h
// 
// 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//=============================================================================

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>

#if defined(DEBUG) || defined(_DEBUG)
// CRTによるメモリーリーク検出
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <wrl.h>
// new 演算子の再定義
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include <stdlib.h>
#include <memory>
#include <wrl.h>
#include <exception>
#include <string>
#include <chrono>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
