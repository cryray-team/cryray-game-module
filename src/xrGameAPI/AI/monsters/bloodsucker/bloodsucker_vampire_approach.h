#pragma once
#include "../state.h"

namespace BLOODSUCKER_VAMPIRE_APPROACH
{
    template <typename _Object>
    class CStateBloodsuckerVampireApproach : public CState<_Object>
    {
        typedef CState<_Object> inherited;

    public:
        CStateBloodsuckerVampireApproach(_Object* obj);
        virtual ~CStateBloodsuckerVampireApproach();

        virtual void initialize();
        virtual void execute();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }
    };
}

namespace BLOODSUCKER3_VAMPIRE_APPROACH
{
	template <typename _Object>
	class CStateBloodsucker3VampireApproach : public CState<_Object>
	{
		typedef CState<_Object> inherited;

	public:
		CStateBloodsucker3VampireApproach(_Object* obj);
		virtual ~CStateBloodsucker3VampireApproach();

		virtual void initialize();
		virtual void execute();
		virtual void remove_links(CObject* object) { inherited::remove_links(object); }
	};
}

#include "bloodsucker_vampire_approach_inline.h"
