// hit_immunity.h: ����� ��� ��� ��������, ������� ������������
//				   ������������ ���������� ��� ������ ����� �����
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../xrGameAPI/alife_space.h"
#include "hit_immunity_space.h"

class GAME_API CHitImmunity
{
    // ������������ �� ������� ����������� ���
    // ��� ��������������� ���� �����������
    //(��� �������� �������� � ����������� ��������)
private:
    HitImmunity::HitTypeSVec m_HitImmunityKoefs;

public:
    CHitImmunity();
    virtual ~CHitImmunity();

    void LoadImmunities(LPCSTR section, CInifile const* ini);
    void AddImmunities(LPCSTR section, CInifile const* ini);
    float GetHitImmunity(ALife::EHitType hit_type) const { return m_HitImmunityKoefs[hit_type]; }
    float AffectHit(float power, ALife::EHitType hit_type) const { return power * GetHitImmunity(hit_type); }
};
