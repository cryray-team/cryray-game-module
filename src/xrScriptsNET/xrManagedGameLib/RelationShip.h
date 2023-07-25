#pragma once
class CHARACTER_COMMUNITY;

// Community Relation Manager
// ForserX and Borshig
namespace XRay
{
	public ref class RelationShip
	{
	internal: 
		CHARACTER_COMMUNITY* pCommunity;
		const char* CommunityName;
		u32 EntityId;
		int LastRSVal;

	public:
		RelationShip(::System::String^ _community, int _entity_id);

		property int CommunityRelationShip
		{
			void set(int Value);
			int get();
		}

		void	Change();

		static int		GetCommunityRelation(::System::String^ comm_from, ::System::String^ comm_to);
		static void		SetCommunityRelation(::System::String^ comm_from, ::System::String^ comm_to, int value);
		static int		GetGeneralGoodwillBetween(u16 from, u16 to);
	};


}
