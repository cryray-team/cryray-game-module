// HUDCrosshair.cpp:  крестик прицела, отображающий текущую дисперсию
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "HUDCrosshair.h"
#include "ui_base.h"
#include "actor.h"
#include "Weapon.h"
#include "player_hud.h"
#include "HUDManager.h"
#include "InventoryOwner.h"
#include "Inventory.h"
#include "../xrEngine/CameraBase.h"

CHUDCrosshair::CHUDCrosshair()
{
    hShader->create("hud\\crosshair");
    radius = 0;
}

CHUDCrosshair::~CHUDCrosshair() {}

void CHUDCrosshair::Load() { cross_color = pSettings->r_fcolor("hud_cursor", "cross_color").get(); }

// выставляет radius от min_radius до max_radius
void CHUDCrosshair::SetDispersion(float disp)
{
    Fvector4 r;
    Fvector R = {VIEWPORT_NEAR * _sin(disp), 0.f, VIEWPORT_NEAR};
    Device.mProject.transform(r, R);

    Fvector2 scr_size;
    scr_size.set(float(xrAPI.Render->getTarget()->get_width()), float(xrAPI.Render->getTarget()->get_height()));
    float radius_pixels = _abs(r.x) * scr_size.x / 2.0f;
    target_radius = radius_pixels;
}

#ifdef DEBUG
void CHUDCrosshair::SetFirstBulletDispertion(float fbdisp)
{
    Fvector4 r;
    Fvector R = {VIEWPORT_NEAR * _sin(fbdisp), 0.f, VIEWPORT_NEAR};
    Device.mProject.transform(r, R);

    Fvector2 scr_size;
    scr_size.set(float(::Render->getTarget()->get_width()), float(::Render->getTarget()->get_height()));
    fb_radius = _abs(r.x) * scr_size.x / 2.0f;
}

BOOL g_bDrawFirstBulletCrosshair = FALSE;

void CHUDCrosshair::OnRenderFirstBulletDispertion()
{
    VERIFY(g_bRendering);
    Fvector2 center;
    Fvector2 scr_size;
    scr_size.set(float(::Render->getTarget()->get_width()), float(::Render->getTarget()->get_height()));
    center.set(scr_size.x / 2.0f, scr_size.y / 2.0f);

    UIRender->StartPrimitive(10, IUIRender::ptLineList, UI().m_currentPointType);

    u32 fb_cross_color = color_rgba(255, 0, 0, 255); // red

    float cross_length = /*cross_length_perc*/ 0.008f * scr_size.x;
    float min_radius = min_radius_perc * scr_size.x;
    float max_radius = max_radius_perc * scr_size.x;

    clamp(target_radius, min_radius, max_radius);

    float x_min = min_radius + fb_radius;
    float x_max = x_min + cross_length;

    float y_min = x_min;
    float y_max = x_max;

    // 0
    UIRender->PushPoint(center.x, center.y + y_min, 0, fb_cross_color, 0, 0);
    UIRender->PushPoint(center.x, center.y + y_max, 0, fb_cross_color, 0, 0);
    // 1
    UIRender->PushPoint(center.x, center.y - y_min, 0, fb_cross_color, 0, 0);
    UIRender->PushPoint(center.x, center.y - y_max, 0, fb_cross_color, 0, 0);
    // 2
    UIRender->PushPoint(center.x + x_min, center.y, 0, fb_cross_color, 0, 0);
    UIRender->PushPoint(center.x + x_max, center.y, 0, fb_cross_color, 0, 0);
    // 3
    UIRender->PushPoint(center.x - x_min, center.y, 0, fb_cross_color, 0, 0);
    UIRender->PushPoint(center.x - x_max, center.y, 0, fb_cross_color, 0, 0);

    // point
    UIRender->PushPoint(center.x - 0.5f, center.y, 0, fb_cross_color, 0, 0);
    UIRender->PushPoint(center.x + 0.5f, center.y, 0, fb_cross_color, 0, 0);

    // render
    UIRender->SetShader(*hShader);
    UIRender->FlushPrimitive();
}
#endif

void CHUDCrosshair::OnRender()
{
    if (p_engine_flags32.test(AF_CROSSHAIR_STANDART))
    {
        Fvector2 center;
        Fvector2 scr_size;
        scr_size.set(float(xrAPI.Render->getTarget()->get_width()), float(xrAPI.Render->getTarget()->get_height()));
        center.set(scr_size.x / 2.0f, scr_size.y / 2.0f);

        xrAPI.UIRender->StartPrimitive(10, IUIRender::ptLineList, UI().m_currentPointType);

        //-' длина крестика
        //-' все размеры в процентах от длины экрана
        float min_radius = (type_crosshair == 0        ? 0.0f :
                                   type_crosshair == 1 ? 0.0f :
                                   type_crosshair == 2 ? 0.0f :
                                                         0.0f) *
            scr_size.x;
        float max_radius = (type_crosshair == 0        ? 1.0f :
                                   type_crosshair == 1 ? 0.0f :
                                   type_crosshair == 2 ? 0.006f :
                                                         0.8f) *
            scr_size.x;
        float cross_length = (type_crosshair == 0        ? 0.015f :
                                     type_crosshair == 1 ? 0.0010f :
                                     type_crosshair == 2 ? 0.0037f :
                                                           0.004f) *
            scr_size.x;

        clamp(target_radius, min_radius, max_radius);

        float x_min = min_radius + radius;
        float x_max = x_min + cross_length;

        float y_min = x_min;
        float y_max = x_max;

        // 0
        xrAPI.UIRender->PushPoint(center.x, center.y + y_min, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x, center.y + y_max, 0, cross_color, 0, 0);
        // 1
        xrAPI.UIRender->PushPoint(center.x, center.y - y_min, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x, center.y - y_max, 0, cross_color, 0, 0);
        // 2
        xrAPI.UIRender->PushPoint(center.x + x_min, center.y, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x + x_max, center.y, 0, cross_color, 0, 0);
        // 3
        xrAPI.UIRender->PushPoint(center.x - x_min, center.y, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x - x_max, center.y, 0, cross_color, 0, 0);

        // point
        xrAPI.UIRender->PushPoint(center.x - 0.5f, center.y, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x + 0.5f, center.y, 0, cross_color, 0, 0);

        // render
        xrAPI.UIRender->SetShader(*hShader);
        xrAPI.UIRender->FlushPrimitive();
    }
    else
    {
        Fvector2 center, scr_size;
        Fvector result;
        Fvector4 v_res;
        float x, y;

        scr_size.set(float(xrAPI.Render->getTarget()->get_width()), float(xrAPI.Render->getTarget()->get_height()));

        CWeapon* weapon = dynamic_cast<CWeapon*>(Actor()->inventory().ActiveItem());
        CCameraBase* pCam = Actor()->cam_Active();
        float dist = HUD().GetCurrentRayQuery().range * 1.2f;

        if (weapon && p_engine_flags32.test(AF_CROSSHAIR_COLLIDE) && !p_engine_flags32.test(AF_CROSSHAIR_INERT))
        {
            result = weapon->get_LastFP();
            result.add(Fvector(Device.vCameraDirection).mul(dist));
        }

        if (p_engine_flags32.test(AF_CROSSHAIR_INERT) && !p_engine_flags32.test(AF_CROSSHAIR_COLLIDE))
        {
            result = pCam->vPosition;
            result.add(Fvector(pCam->vDirection).mul(dist));
        }

        Device.mFullTransform.transform(v_res, result);

        x = (1.f + v_res.x) / 2.f * (Device.dwWidth);
        y = (1.f - v_res.y) / 2.f * (Device.dwHeight);

        if ((p_engine_flags32.test(AF_CROSSHAIR_INERT) || p_engine_flags32.test(AF_CROSSHAIR_COLLIDE)) &&
            !(p_engine_flags32.test(AF_CROSSHAIR_INERT) && p_engine_flags32.test(AF_CROSSHAIR_COLLIDE)))
            center.set(x, y);
        else
            center.set(scr_size.x / 2.0f, scr_size.y / 2.0f);

        xrAPI.UIRender->StartPrimitive(10, IUIRender::ptLineList, UI().m_currentPointType);

        //-' длина крестика
        //-' все размеры в процентах от длины экрана
        float min_radius = (type_crosshair == 0        ? 0.0f :
                                   type_crosshair == 1 ? 0.0f :
                                   type_crosshair == 2 ? 0.0f :
                                                         0.0f) *
            scr_size.x;
        float max_radius = (type_crosshair == 0        ? 1.0f :
                                   type_crosshair == 1 ? 0.0f :
                                   type_crosshair == 2 ? 0.006f :
                                                         0.8f) *
            scr_size.x;
        float cross_length = (type_crosshair == 0        ? 0.015f :
                                     type_crosshair == 1 ? 0.0010f :
                                     type_crosshair == 2 ? 0.0037f :
                                                           0.004f) *
            scr_size.x;

        clamp(target_radius, min_radius, max_radius);

        float x_min = min_radius + radius;
        float x_max = x_min + cross_length;

        float y_min = x_min;
        float y_max = x_max;

        // 0
        xrAPI.UIRender->PushPoint(center.x, center.y + y_min, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x, center.y + y_max, 0, cross_color, 0, 0);
        // 1
        xrAPI.UIRender->PushPoint(center.x, center.y - y_min, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x, center.y - y_max, 0, cross_color, 0, 0);
        // 2
        xrAPI.UIRender->PushPoint(center.x + x_min, center.y, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x + x_max, center.y, 0, cross_color, 0, 0);
        // 3
        xrAPI.UIRender->PushPoint(center.x - x_min, center.y, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x - x_max, center.y, 0, cross_color, 0, 0);

        // point
        xrAPI.UIRender->PushPoint(center.x - 0.5f, center.y, 0, cross_color, 0, 0);
        xrAPI.UIRender->PushPoint(center.x + 0.5f, center.y, 0, cross_color, 0, 0);

        // render
        xrAPI.UIRender->SetShader(*hShader);
        xrAPI.UIRender->FlushPrimitive();
    }

    if (!fsimilar(target_radius, radius))
    {
        radius = target_radius;
    };
#ifdef DEBUG
    if (g_bDrawFirstBulletCrosshair)
        OnRenderFirstBulletDispertion();
#endif
}