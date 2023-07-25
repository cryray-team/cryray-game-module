#pragma once
namespace GameGraph { class CVertex; }

namespace XRay
{
	public value class GameVertex sealed
	{
		GameGraph::CVertex* pNativeObject;
	public:
		GameVertex(IntPtr InNativeObject);

		property ::System::Numerics::Vector3 LevelPoint
		{
			::System::Numerics::Vector3 get();
		}

		property ::System::Numerics::Vector3 GamePoint
		{
			::System::Numerics::Vector3 get();
		}

		property ::System::Byte LevelID
		{
			::System::Byte get();
		}

		property ::System::UInt32 LevelVertexID
		{
			::System::UInt32 get();
		}

		property array<::System::Byte>^ Mask
		{
			array<::System::Byte>^ get();
		}

		property ::System::Byte EdgeCount
		{
			::System::Byte get();
		}

		property ::System::UInt32 EdgeOffset
		{
			::System::UInt32 get();
		}
	};
}