#pragma once

/* Переменные которые должны синхронно накапливаться или убывать, нужны для юнити сборки xrGame*/
namespace CryRayStaticValues
{
	static float depth = 0.0f;

	static u32 const time_to_delete_smart_cover = 300000;

	constexpr float MIN_SUITABLE_ENEMY_DISTANCE = 3.f;
} // namespace CryRayStaticValues
