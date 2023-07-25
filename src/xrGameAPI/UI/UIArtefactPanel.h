#pragma once
#include "ui/UIWindow.h"
#include "UIStaticItem.h"

class CUIXml;
class CArtefact;
class CUIStaticItem;

namespace UIArtefactPanel
{
    class GAME_API CUIArtefactPanel : public CUIWindow
    {
    protected:
        float m_fScale;
        Fvector2 m_cell_size;
        xr_vector<Frect> m_vRects;
        CUIStaticItem m_StaticItem;

    public:
        CUIArtefactPanel();
        ~CUIArtefactPanel();

        void InitIcons(const xr_vector<const CArtefact*>& artefacts);
        void InitFromXML(CUIXml& xml, const char* path, int index);
        virtual void Draw() override;
    };
} // namespace UIArtefactPanel