#pragma once

//-' CryRay Engine x64
//-' Глобальный конфиг дефайнов

//-' Глобальные включения
	#include <regex>
	#include <string>
	#include <algorithm>
#ifndef SPECTRE
	#include <execution>
#endif
//-' Защита от вылета, когда аргумент равен нулю или исключению(DEBUG)
	#define CRASH_PROTECTION(cond, ...) do { if (!(cond)) Msg(__VA_ARGS__); } while (0)	

//-' ASSERT как в скриптах, с поддержкой форматирования строки и неограниченным кол-вом аргументов
	#define ASSERT_FMT(expr, ...) do { \
	if (!(expr)) \
		FATAL(__VA_ARGS__); \
	} while(0)

//-' 0xED - max vavue in DIK* enum
	#define MOUSE_1 (0xED + 100)
	#define MOUSE_2 (0xED + 101)
	#define MOUSE_3 (0xED + 102)
	#define MOUSE_4 (0xED + 103)
	#define MOUSE_5 (0xED + 104)
	#define MOUSE_6 (0xED + 105)
	#define MOUSE_7 (0xED + 106)
	#define MOUSE_8 (0xED + 107)

//--' Шифратор шейдеров
//--' Инструкция использования: 
	//* Назначить xrShaderCryptor исполняемым проектом 
	// *** Поднастройка проекта: 
	// * В свойствах xrShaderCryptor, перейти в "Свойства конфигурации" -> Отладка 
	// * В аргументы команды указать: -out="Диск:\Users\OldSerpskiStalker\Documents\GitHub\Engine_CryRay_x64\src\xrRender\generated" -arch=x64 -vsdir="C:\Program Files\Microsoft Visual Studio\2022\Community"
	// * -vsdir= не обязателен, вместо него можно создать глобальную среду.
	// * Рабочий каталог: Диск:\[CryRay] Call of Chernobyl Back to the Roots
	//*********************************************************************** 
	//* ValeroK: Дописал 3й аргумент с путем к VisualStudio, если указан -vsdir= то следующие пункты можно не делать
	//* {
	//*		Создание глобальной среды:
	//*
	//*		Настройка активации: Панель управления -> Система и Безопасность -> Система -> Дополнительные параметры системы -> Переменные среды 
	//*		Зайти в "Переменные среды" 
	//*		Вкладка: "Системные переменные", нажать создать 
	//*		Высветится окно: Изменение системной переменной: 
	//*		Имя переменной: VS2022INSTALLDIR 
	//*		Значение переменной: C:\Program Files\Microsoft Visual Studio\2022\Community\ -> указать путь 
	//*		Перезагрузить компьютер для активации новой среды 
	//* }
	//* Запустить Локальный отладчик Windows с xrShaderCryptor //-' Back to the Roots
	// 
	// * Шифратор может работать как отдельная программа
	// * Для запуска без отладчика нужно создать отдельную папку с шифратором
	// * Поместить xrShaderCryptor.exe, xrCore.dll, bugtrap.dll, xrAPI.dll в созданную папку
	// * Создать fsgame.ltx с аргументами:
	// * $app_data_root$		= true|		false|  $fs_root$|          appdata\
	// * $game_shaders$    		= true|		true|	$fs_root$|			shaders\
	// * Сделать батник запуска:
	// * @start xrShaderCryptor -out="Диск:\Users\OldSerpskiStalker\Documents\GitHub\Engine_CryRay_x64\src\xrRender\generated" -arch=x64 -vsdir="C:\Program Files\Microsoft Visual Studio\2022\Community"

	//#define ACTIVE_VERIFY																  // Активировать режим точности ошибок в релизе
	#define ALIFE_VERSION				0x0899										      // Alife

//-' NET_CLIENT, потом избавится от этого через коммиты OGSR - Файлы: Net_Client.cpp и NET_Server.cpp
	#define START_PORT_LAN_SV			5445 + 1
	#define START_PORT					0
	#define END_PORT					65535
	#define END_PORT_LAN				5445 + 250
	#define START_PORT_LAN_CL			5445 + 2

//-' CORE
	#define NON_FATAL_VERIFY								// don't crash game when VERIFY fails
//-' CORE

/*
//-' LAYERED_SND_SHOOT by Alundaio
	When defined, it will allow you to play a group of sounds from a specified section for snd_shoot.
	You can have as many layers as you want, but you must follow naming convention,

	snd_1_layer
	snd_2_layer
	snd_3_layer
	...

	You can also have different variants for each layer defined,

	snd_1_layer
	snd_1_layer1
	snd_1_layer2
	...

	The correct line settings are standard, (ie. snd_1_layer = sound_path, volume, delay)
	ex.

	Here is an example usage:
	snd_shoot = new_snd_section

	[new_snd_section]
	snd_1_layer = weapons\new_sound_shoot1
	snd_1_layer1 = weapons\new_sound_shoot2
	snd_1_layer2 = weapons\new_sound_shoot3
	snd_1_layer3 = weapons\new_sound_shoot4

	snd_2_layer = weapons\mechanical_noise, 1.0, 0.1

	snd_3_layer = weapons\gunshot_echo, 1.0, 0.8
*/