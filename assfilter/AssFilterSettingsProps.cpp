/*
 *   Copyright(C) 2016 Blitzker
 *
 *   This program is free software : you can redistribute it and / or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"
#include "resource.h"
#include <Commctrl.h>
#include "AssFilterSettingsProps.h"
#include "registry.h"
#include "Tools.h"
#include "version.h"

CAssFilterSettingsProp::CAssFilterSettingsProp(LPUNKNOWN pUnk, HRESULT *phr) :
    CBasePropertyPage(NAME("AssFilterSettingsProp"), pUnk, IDD_PROPPAGE_SETTINGS, IDS_PROPPAGE_SETTINGS_TITLE),
    m_acrCustClr {}
{
}

CAssFilterSettingsProp::~CAssFilterSettingsProp()
{
}

CUnknown * WINAPI CAssFilterSettingsProp::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr)
{
    CAssFilterSettingsProp *pNewObject = new CAssFilterSettingsProp(pUnk, pHr);
    if (pNewObject == nullptr)
    {
        *pHr = E_OUTOFMEMORY;
    }
    return pNewObject;
}

HRESULT CAssFilterSettingsProp::OnConnect(IUnknown *pUnk)
{
    if (pUnk == nullptr)
    {
        return E_POINTER;
    }
    ASSERT(m_pAssFilterSettings == nullptr);
    return pUnk->QueryInterface(&m_pAssFilterSettings);
}

HRESULT CAssFilterSettingsProp::OnDisconnect(void)
{
    m_pAssFilterSettings->Release();
    m_pAssFilterSettings = nullptr;

    return S_OK;
}

HRESULT CAssFilterSettingsProp::OnActivate(void)
{
    HRESULT hr = S_OK;
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_BAR_CLASSES | ICC_STANDARD_CLASSES;
    if (InitCommonControlsEx(&icc) == FALSE)
    {
        return E_FAIL;
    }

    ASSERT(m_pAssFilterSettings != nullptr);

    hr = LoadSettings();
    if (SUCCEEDED(hr))
    {
        SendDlgItemMessage(m_Dlg, IDC_FONT, WM_SETTEXT, 0, (LPARAM)m_settings.FontName.c_str());

        WCHAR stringBuffer[100];
        swprintf_s(stringBuffer, L"%u", m_settings.FontScaleX);
        SendDlgItemMessage(m_Dlg, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN1, UDM_SETRANGE32, 0, 1000);

        swprintf_s(stringBuffer, L"%u", m_settings.FontScaleY);
        SendDlgItemMessage(m_Dlg, IDC_EDIT2, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN2, UDM_SETRANGE32, 0, 1000);

        swprintf_s(stringBuffer, L"%u", m_settings.FontSpacing);
        SendDlgItemMessage(m_Dlg, IDC_EDIT3, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN3, UDM_SETRANGE32, 0, 100);

        swprintf_s(stringBuffer, L"%u", m_settings.FontBlur);
        SendDlgItemMessage(m_Dlg, IDC_EDIT4, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN4, UDM_SETRANGE32, 0, 100);

        swprintf_s(stringBuffer, L"%u", m_settings.FontOutline);
        SendDlgItemMessage(m_Dlg, IDC_EDIT5, WM_SETTEXT, 0, (LPARAM)stringBuffer);

        swprintf_s(stringBuffer, L"%u", m_settings.FontShadow);
        SendDlgItemMessage(m_Dlg, IDC_EDIT6, WM_SETTEXT, 0, (LPARAM)stringBuffer);

        switch (m_settings.LineAlignment)
        {
        case 1:
            SendDlgItemMessage(m_Dlg, IDC_RADIO7, BM_SETCHECK, BST_CHECKED, 0);
            break;
        case 2:
            SendDlgItemMessage(m_Dlg, IDC_RADIO8, BM_SETCHECK, BST_CHECKED, 0);
            break;
        case 3:
            SendDlgItemMessage(m_Dlg, IDC_RADIO9, BM_SETCHECK, BST_CHECKED, 0);
            break;
        case 4:
            SendDlgItemMessage(m_Dlg, IDC_RADIO4, BM_SETCHECK, BST_CHECKED, 0);
            break;
        case 5:
            SendDlgItemMessage(m_Dlg, IDC_RADIO5, BM_SETCHECK, BST_CHECKED, 0);
            break;
        case 6:
            SendDlgItemMessage(m_Dlg, IDC_RADIO6, BM_SETCHECK, BST_CHECKED, 0);
            break;
        case 7:
            SendDlgItemMessage(m_Dlg, IDC_RADIO1, BM_SETCHECK, BST_CHECKED, 0);
            break;
        case 8:
            SendDlgItemMessage(m_Dlg, IDC_RADIO2, BM_SETCHECK, BST_CHECKED, 0);
            break;
        case 9:
            SendDlgItemMessage(m_Dlg, IDC_RADIO3, BM_SETCHECK, BST_CHECKED, 0);
            break;
        default:
            m_settings.LineAlignment = 2;
            SendDlgItemMessage(m_Dlg, IDC_RADIO8, BM_SETCHECK, BST_CHECKED, 0);
            break;
        }

        swprintf_s(stringBuffer, L"%d", m_settings.MarginLeft);
        SendDlgItemMessage(m_Dlg, IDC_EDIT7, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN5, UDM_SETRANGE32, 0, 1000);

        swprintf_s(stringBuffer, L"%d", m_settings.MarginRight);
        SendDlgItemMessage(m_Dlg, IDC_EDIT8, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN6, UDM_SETRANGE32, 0, 1000);

        swprintf_s(stringBuffer, L"%d", m_settings.MarginVertical);
        SendDlgItemMessage(m_Dlg, IDC_EDIT9, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN7, UDM_SETRANGE32, 0, 1000);

        swprintf_s(stringBuffer, L"%u", (m_settings.ColorPrimary >> 24));
        SendDlgItemMessage(m_Dlg, IDC_EDIT10, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN8, UDM_SETRANGE32, 0, 255);

        swprintf_s(stringBuffer, L"%u", (m_settings.ColorSecondary >> 24));
        SendDlgItemMessage(m_Dlg, IDC_EDIT12, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN9, UDM_SETRANGE32, 0, 255);

        swprintf_s(stringBuffer, L"%u", (m_settings.ColorOutline >> 24));
        SendDlgItemMessage(m_Dlg, IDC_EDIT13, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN10, UDM_SETRANGE32, 0, 255);

        swprintf_s(stringBuffer, L"%u", (m_settings.ColorShadow >> 24));
        SendDlgItemMessage(m_Dlg, IDC_EDIT14, WM_SETTEXT, 0, (LPARAM)stringBuffer);
        SendDlgItemMessage(m_Dlg, IDC_SPIN11, UDM_SETRANGE32, 0, 255);

        SendDlgItemMessage(m_Dlg, IDC_EDIT11, WM_SETTEXT, 0, (LPARAM)m_settings.CustomTags.c_str());

        SendDlgItemMessage(m_Dlg, IDC_CHECK1, BM_SETCHECK, m_settings.NativeSize, 0);

        const WCHAR* customResolution[9] = { L"Video", L"3840x2160", L"2560x1440",
                                             L"1920x1080", L"1440x900", L"1280x720",
                                             L"1024x768", L"800x600", L"640x480" };
        SendDlgItemMessage(m_Dlg, IDC_COMBO1, CB_RESETCONTENT, 0, 0);
        for (int i = 0; i < 9; i++)
            SendDlgItemMessage(m_Dlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)customResolution[i]);
        SendDlgItemMessage(m_Dlg, IDC_COMBO1, CB_SETCURSEL, m_settings.CustomRes, 0);
        EnableWindow(GetDlgItem(m_Dlg, IDC_COMBO1), m_settings.NativeSize);
    }

    // Erase the names of the custom draw buttons
    SendDlgItemMessage(m_Dlg, IDC_BUTTON1, WM_SETTEXT, 0, (LPARAM)L"");
    SendDlgItemMessage(m_Dlg, IDC_BUTTON2, WM_SETTEXT, 0, (LPARAM)L"");
    SendDlgItemMessage(m_Dlg, IDC_BUTTON3, WM_SETTEXT, 0, (LPARAM)L"");
    SendDlgItemMessage(m_Dlg, IDC_BUTTON4, WM_SETTEXT, 0, (LPARAM)L"");

    return hr;
}

HRESULT CAssFilterSettingsProp::OnApplyChanges(void)
{
    ASSERT(m_pAssFilterSettings != nullptr);
    HRESULT hr = S_OK;

    // Sanity check before saving the settings
    m_settings.NativeSize = (BOOL)SendDlgItemMessage(m_Dlg, IDC_CHECK1, BM_GETCHECK, 0, 0);

    if (m_settings.FontName.empty())
        m_settings.FontName.assign(L"Arial");

    if (m_settings.FontSize == 0 || m_settings.FontSize > 200)
        m_settings.FontSize = 50;

    WCHAR wsBuffer[10];
    SendDlgItemMessage(m_Dlg, IDC_EDIT1, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    int iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 1 || iBuffer > 1000)
        iBuffer = 100;
    m_settings.FontScaleX = (DWORD)iBuffer;

    SendDlgItemMessage(m_Dlg, IDC_EDIT2, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 1 || iBuffer > 1000)
        iBuffer = 100;
    m_settings.FontScaleY = (DWORD)iBuffer;

    SendDlgItemMessage(m_Dlg, IDC_EDIT3, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 100)
        iBuffer = 0;
    m_settings.FontSpacing = (DWORD)iBuffer;

    SendDlgItemMessage(m_Dlg, IDC_EDIT4, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 100)
        iBuffer = 0;
    m_settings.FontBlur = (DWORD)iBuffer;

    SendDlgItemMessage(m_Dlg, IDC_EDIT5, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 100)
        iBuffer = 0;
    m_settings.FontOutline = (DWORD)iBuffer;

    SendDlgItemMessage(m_Dlg, IDC_EDIT6, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 100)
        iBuffer = 0;
    m_settings.FontShadow = (DWORD)iBuffer;

    if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO1, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 7;
    else if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO2, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 8;
    else if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO3, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 9;
    else if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO4, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 4;
    else if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO5, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 5;
    else if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO6, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 6;
    else if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO7, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 1;
    else if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO8, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 2;
    else if ((BOOL)SendDlgItemMessage(m_Dlg, IDC_RADIO9, BM_GETCHECK, 0, 0))
        m_settings.LineAlignment = 3;
    else
        m_settings.LineAlignment = 2;

    SendDlgItemMessage(m_Dlg, IDC_EDIT7, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 1000)
        iBuffer = 0;
    m_settings.MarginLeft = (DWORD)iBuffer;

    SendDlgItemMessage(m_Dlg, IDC_EDIT8, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 1000)
        iBuffer = 0;
    m_settings.MarginRight = (DWORD)iBuffer;

    SendDlgItemMessage(m_Dlg, IDC_EDIT9, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 1000)
        iBuffer = 0;
    m_settings.MarginVertical = (DWORD)iBuffer;

    SendDlgItemMessage(m_Dlg, IDC_EDIT10, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 255)
        iBuffer = 0;
    m_settings.ColorPrimary = (m_settings.ColorPrimary & 0x00FFFFFF) | (DWORD)iBuffer << 24;

    SendDlgItemMessage(m_Dlg, IDC_EDIT12, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 255)
        iBuffer = 0;
    m_settings.ColorSecondary = (m_settings.ColorSecondary & 0x00FFFFFF) | (DWORD)iBuffer << 24;

    SendDlgItemMessage(m_Dlg, IDC_EDIT13, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 255)
        iBuffer = 0;
    m_settings.ColorOutline = (m_settings.ColorOutline & 0x00FFFFFF) | (DWORD)iBuffer << 24;

    SendDlgItemMessage(m_Dlg, IDC_EDIT14, WM_GETTEXT, 10, (LPARAM)&wsBuffer);
    iBuffer = _wtoi(wsBuffer);
    if (iBuffer < 0 || iBuffer > 255)
        iBuffer = 0;
    m_settings.ColorShadow = (m_settings.ColorShadow & 0x00FFFFFF) | (DWORD)iBuffer << 24;

    m_settings.CustomRes = (DWORD)SendDlgItemMessage(m_Dlg, IDC_COMBO1, CB_GETCURSEL, 0, 0);

    WCHAR wsCustomBuffer[1024];
    SendDlgItemMessage(m_Dlg, IDC_EDIT11, WM_GETTEXT, 1024, (LPARAM)&wsCustomBuffer);
    m_settings.CustomTags.assign(wsCustomBuffer);

    return SaveSettings();
}

HRESULT CAssFilterSettingsProp::LoadSettings()
{
    HRESULT hr;
    DWORD dwVal;
    BOOL bFlag;
    std::wstring strVal;

    CRegistry reg = CRegistry(HKEY_CURRENT_USER, ASSFILTER_REGISTRY_KEY, hr, TRUE);
    if (SUCCEEDED(hr)) {
        bFlag = reg.ReadBOOL(L"TrayIcon", hr);
        if (SUCCEEDED(hr)) m_settings.TrayIcon = bFlag;

        bFlag = reg.ReadBOOL(L"NativeSize", hr);
        if (SUCCEEDED(hr)) m_settings.NativeSize = bFlag;

        strVal = reg.ReadString(L"FontName", hr);
        if (SUCCEEDED(hr)) m_settings.FontName = strVal;

        dwVal = reg.ReadDWORD(L"FontSize", hr);
        if (SUCCEEDED(hr)) m_settings.FontSize = dwVal;

        dwVal = reg.ReadDWORD(L"FontScaleX", hr);
        if (SUCCEEDED(hr)) m_settings.FontScaleX = dwVal;

        dwVal = reg.ReadDWORD(L"FontScaleY", hr);
        if (SUCCEEDED(hr)) m_settings.FontScaleY = dwVal;

        dwVal = reg.ReadDWORD(L"FontSpacing", hr);
        if (SUCCEEDED(hr)) m_settings.FontSpacing = dwVal;

        dwVal = reg.ReadDWORD(L"FontBlur", hr);
        if (SUCCEEDED(hr)) m_settings.FontBlur = dwVal;

        dwVal = reg.ReadDWORD(L"FontOutline", hr);
        if (SUCCEEDED(hr)) m_settings.FontOutline = dwVal;

        dwVal = reg.ReadDWORD(L"FontShadow", hr);
        if (SUCCEEDED(hr)) m_settings.FontShadow = dwVal;

        dwVal = reg.ReadDWORD(L"LineAlignment", hr);
        if (SUCCEEDED(hr)) m_settings.LineAlignment = dwVal;

        dwVal = reg.ReadDWORD(L"MarginLeft", hr);
        if (SUCCEEDED(hr)) m_settings.MarginLeft = dwVal;

        dwVal = reg.ReadDWORD(L"MarginRight", hr);
        if (SUCCEEDED(hr)) m_settings.MarginRight = dwVal;

        dwVal = reg.ReadDWORD(L"MarginVertical", hr);
        if (SUCCEEDED(hr)) m_settings.MarginVertical = dwVal;

        dwVal = reg.ReadDWORD(L"ColorPrimary", hr);
        if (SUCCEEDED(hr)) m_settings.ColorPrimary = dwVal;

        dwVal = reg.ReadDWORD(L"ColorSecondary", hr);
        if (SUCCEEDED(hr)) m_settings.ColorSecondary = dwVal;

        dwVal = reg.ReadDWORD(L"ColorOutline", hr);
        if (SUCCEEDED(hr)) m_settings.ColorOutline = dwVal;

        dwVal = reg.ReadDWORD(L"ColorShadow", hr);
        if (SUCCEEDED(hr)) m_settings.ColorShadow = dwVal;

        dwVal = reg.ReadDWORD(L"CustomRes", hr);
        if (SUCCEEDED(hr)) m_settings.CustomRes = dwVal;

        strVal = reg.ReadString(L"CustomTags", hr);
        if (SUCCEEDED(hr)) m_settings.CustomTags = strVal;
    }

    return S_OK;
}

HRESULT CAssFilterSettingsProp::LoadDefaults()
{
    m_settings.TrayIcon = FALSE;
    m_settings.NativeSize = FALSE;

    m_settings.FontName = L"Arial";
    m_settings.FontSize = 50;
    m_settings.FontScaleX = 100;
    m_settings.FontScaleY = 100;
    m_settings.FontSpacing = 0;
    m_settings.FontBlur = 0;

    m_settings.FontOutline = 2;
    m_settings.FontShadow = 0;
    m_settings.LineAlignment = 2;
    m_settings.MarginLeft = 120;
    m_settings.MarginRight = 120;
    m_settings.MarginVertical = 20;
    m_settings.ColorPrimary = 0x00FFFFFF;
    m_settings.ColorSecondary = 0x00FFFF;
    m_settings.ColorOutline = 0;
    m_settings.ColorShadow = 0;
    m_settings.CustomRes = 0;

    m_settings.CustomTags = L"";

    SendDlgItemMessage(m_Dlg, IDC_FONT, WM_SETTEXT, 0, (LPARAM)m_settings.FontName.c_str());

    WCHAR stringBuffer[100];
    swprintf_s(stringBuffer, L"%u", m_settings.FontScaleX);
    SendDlgItemMessage(m_Dlg, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", m_settings.FontScaleY);
    SendDlgItemMessage(m_Dlg, IDC_EDIT2, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", m_settings.FontSpacing);
    SendDlgItemMessage(m_Dlg, IDC_EDIT3, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", m_settings.FontBlur);
    SendDlgItemMessage(m_Dlg, IDC_EDIT4, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", m_settings.FontOutline);
    SendDlgItemMessage(m_Dlg, IDC_EDIT5, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", m_settings.FontShadow);
    SendDlgItemMessage(m_Dlg, IDC_EDIT6, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    m_settings.LineAlignment = 2;
    SendDlgItemMessage(m_Dlg, IDC_RADIO8, BM_SETCHECK, BST_CHECKED, 0);

    swprintf_s(stringBuffer, L"%d", m_settings.MarginLeft);
    SendDlgItemMessage(m_Dlg, IDC_EDIT7, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%d", m_settings.MarginRight);
    SendDlgItemMessage(m_Dlg, IDC_EDIT8, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%d", m_settings.MarginVertical);
    SendDlgItemMessage(m_Dlg, IDC_EDIT9, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", (m_settings.ColorPrimary >> 24));
    SendDlgItemMessage(m_Dlg, IDC_EDIT10, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", (m_settings.ColorSecondary >> 24));
    SendDlgItemMessage(m_Dlg, IDC_EDIT12, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", (m_settings.ColorOutline >> 24));
    SendDlgItemMessage(m_Dlg, IDC_EDIT13, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    swprintf_s(stringBuffer, L"%u", (m_settings.ColorShadow >> 24));
    SendDlgItemMessage(m_Dlg, IDC_EDIT14, WM_SETTEXT, 0, (LPARAM)stringBuffer);

    SendDlgItemMessage(m_Dlg, IDC_EDIT11, WM_SETTEXT, 0, (LPARAM)m_settings.CustomTags.c_str());

    SendDlgItemMessage(m_Dlg, IDC_CHECK1, BM_SETCHECK, m_settings.NativeSize, 0);

    SendDlgItemMessage(m_Dlg, IDC_COMBO1, CB_SETCURSEL, m_settings.CustomRes, 0);
    EnableWindow(GetDlgItem(m_Dlg, IDC_COMBO1), m_settings.NativeSize);

    return S_OK;
}

HRESULT CAssFilterSettingsProp::SaveSettings()
{
    HRESULT hr;
    CreateRegistryKey(HKEY_CURRENT_USER, ASSFILTER_REGISTRY_KEY);
    CRegistry reg = CRegistry(HKEY_CURRENT_USER, ASSFILTER_REGISTRY_KEY, hr);
    if (SUCCEEDED(hr)) {
        reg.WriteBOOL(L"TrayIcon", m_settings.TrayIcon);
        reg.WriteBOOL(L"NativeSize", m_settings.NativeSize);
        reg.WriteString(L"FontName", m_settings.FontName.c_str());
        reg.WriteDWORD(L"FontSize", m_settings.FontSize);
        reg.WriteDWORD(L"FontScaleX", m_settings.FontScaleX);
        reg.WriteDWORD(L"FontScaleY", m_settings.FontScaleY);
        reg.WriteDWORD(L"FontSpacing", m_settings.FontSpacing);
        reg.WriteDWORD(L"FontBlur", m_settings.FontBlur);
        reg.WriteDWORD(L"FontOutline", m_settings.FontOutline);
        reg.WriteDWORD(L"FontShadow", m_settings.FontShadow);
        reg.WriteDWORD(L"LineAlignment", m_settings.LineAlignment);
        reg.WriteDWORD(L"MarginLeft", m_settings.MarginLeft);
        reg.WriteDWORD(L"MarginRight", m_settings.MarginRight);
        reg.WriteDWORD(L"MarginVertical", m_settings.MarginVertical);
        reg.WriteDWORD(L"ColorPrimary", m_settings.ColorPrimary);
        reg.WriteDWORD(L"ColorSecondary", m_settings.ColorSecondary);
        reg.WriteDWORD(L"ColorOutline", m_settings.ColorOutline);
        reg.WriteDWORD(L"ColorShadow", m_settings.ColorShadow);
        reg.WriteDWORD(L"CustomRes", m_settings.CustomRes);
        reg.WriteString(L"CustomTags", m_settings.CustomTags.c_str());
    }

    return S_OK;
}

void CAssFilterSettingsProp::DrawCustomButton(LPNMCUSTOMDRAW &lpnmCD, COLORREF crPen, COLORREF crBrush)
{
    // Select background color
    HBRUSH currentbrush = CreateSolidBrush(crBrush & 0x00FFFFFF);

    // Create pen for button border
    HPEN currentpen = CreatePen(PS_INSIDEFRAME, 2, crPen & 0x00FFFFFF);

    // Select our brush into hDC
    HGDIOBJ old_pen = SelectObject(lpnmCD->hdc, currentpen);
    HGDIOBJ old_brush = SelectObject(lpnmCD->hdc, currentbrush);

    // Draw our button.
    Rectangle(lpnmCD->hdc, lpnmCD->rc.left + 1, lpnmCD->rc.top + 1, lpnmCD->rc.right - 1, lpnmCD->rc.bottom - 1);

    // Clean up
    SelectObject(lpnmCD->hdc, old_pen);
    SelectObject(lpnmCD->hdc, old_brush);
    DeleteObject(currentpen);
    DeleteObject(currentbrush);
}

INT_PTR CAssFilterSettingsProp::OnReceiveMessage(HWND hwnd,
    UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_CHECK1 && HIWORD(wParam) == BN_CLICKED)
        {
            SetDirty();
            EnableWindow(GetDlgItem(m_Dlg, IDC_COMBO1), (BOOL)SendDlgItemMessage(m_Dlg, IDC_CHECK1, BM_GETCHECK, 0, 0));
        }
        else if (LOWORD(wParam) == IDC_FONT)
        {
            CHOOSEFONT cf {};
            LOGFONT lf {};
            HDC hdc = GetWindowDC(hwnd);

            // Initialize CHOOSEFONT
            cf.hwndOwner = hwnd;
            cf.lStructSize = sizeof(CHOOSEFONT);
            cf.lpLogFont = &lf;
            cf.Flags = CF_EFFECTS | CF_NOSCRIPTSEL | CF_INITTOLOGFONTSTRUCT | CF_INACTIVEFONTS;
            lf.lfHeight = -MulDiv(m_settings.FontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            wcscpy_s(lf.lfFaceName, m_settings.FontName.c_str());

            if (ChooseFont(&cf))
            {
                m_settings.FontName.assign(lf.lfFaceName);
                m_settings.FontSize = cf.iPointSize / 10;
                SendDlgItemMessage(m_Dlg, IDC_FONT, WM_SETTEXT, 0, (LPARAM)m_settings.FontName.c_str());
                SetDirty();
            }

            return (LRESULT)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON1)
        {
            CHOOSECOLOR cc {};

            // Initialize CHOOSECOLOR 
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hwnd;
            cc.lpCustColors = (LPDWORD)m_acrCustClr;
            cc.rgbResult = m_settings.ColorPrimary & 0x00FFFFFF;
            cc.Flags = CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN;

            if (ChooseColor(&cc))
            {
                m_settings.ColorPrimary = (m_settings.ColorPrimary & 0xFF000000) | cc.rgbResult;
                InvalidateRect(hwnd, NULL, FALSE);
                SetDirty();
            }

            return (LRESULT)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)
        {
            CHOOSECOLOR cc {};

            // Initialize CHOOSECOLOR 
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hwnd;
            cc.lpCustColors = (LPDWORD)m_acrCustClr;
            cc.rgbResult = m_settings.ColorSecondary & 0x00FFFFFF;
            cc.Flags = CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN;

            if (ChooseColor(&cc))
            {
                m_settings.ColorSecondary = (m_settings.ColorSecondary & 0xFF000000) | cc.rgbResult;
                InvalidateRect(hwnd, NULL, FALSE);
                SetDirty();
            }

            return (LRESULT)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)
        {
            CHOOSECOLOR cc {};

            // Initialize CHOOSECOLOR 
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hwnd;
            cc.lpCustColors = (LPDWORD)m_acrCustClr;
            cc.rgbResult = m_settings.ColorOutline & 0x00FFFFFF;
            cc.Flags = CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN;

            if (ChooseColor(&cc))
            {
                m_settings.ColorOutline = (m_settings.ColorOutline & 0xFF000000) | cc.rgbResult;
                InvalidateRect(hwnd, NULL, FALSE);
                SetDirty();
            }

            return (LRESULT)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON4)
        {
            CHOOSECOLOR cc {};

            // Initialize CHOOSECOLOR 
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hwnd;
            cc.lpCustColors = (LPDWORD)m_acrCustClr;
            cc.rgbResult = m_settings.ColorShadow & 0x00FFFFFF;
            cc.Flags = CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN;

            if (ChooseColor(&cc))
            {
                m_settings.ColorShadow = (m_settings.ColorShadow & 0xFF000000) | cc.rgbResult;
                InvalidateRect(hwnd, NULL, FALSE);
                SetDirty();
            }

            return (LRESULT)TRUE;
        }
        else if (LOWORD(wParam) == IDC_EDIT1 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 1000)
                value = 1000;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 1000)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.FontScaleX)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT2 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 1000)
                value = 1000;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 1000)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.FontScaleY)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT3 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 100)
                value = 100;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 100)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.FontSpacing)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT4 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 100)
                value = 100;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 100)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.FontBlur)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT5 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 100)
                value = 100;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 100)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.FontOutline)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT6 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 100)
                value = 100;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 100)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.FontShadow)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT7 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 1000)
                value = 1000;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 1000)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.MarginLeft)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT8 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 1000)
                value = 1000;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 1000)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.MarginRight)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT9 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 1000)
                value = 1000;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 1000)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.MarginVertical)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT10 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 255)
                value = 255;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 255)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.ColorPrimary >> 24)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT11 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR wsCustomBuffer[1024];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 1024, (LPARAM)&wsCustomBuffer);
            if (wcscmp(wsCustomBuffer, m_settings.CustomTags.c_str()) != 0)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT12 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 255)
                value = 255;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 255)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.ColorSecondary >> 24)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT13 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 255)
                value = 255;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 255)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.ColorOutline >> 24)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_EDIT14 && HIWORD(wParam) == EN_CHANGE)
        {
            WCHAR buffer[10];
            SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_GETTEXT, 10, (LPARAM)&buffer);
            int value = _wtoi(buffer);
            int oldvalue = value;
            size_t len = wcslen(buffer);
            if (value > 255)
                value = 255;
            swprintf_s(buffer, L"%d", value);
            if (wcslen(buffer) != len || oldvalue > 255)
                SendDlgItemMessage(m_Dlg, LOWORD(wParam), WM_SETTEXT, 0, (LPARAM)buffer);
            if (value != m_settings.ColorShadow >> 24)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO1 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 7)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO2 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 8)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO3 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 9)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO4 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 4)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO5 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 5)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO6 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 6)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO7 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 1)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO8 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 2)
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_RADIO9 && HIWORD(wParam) == BN_CLICKED)
        {
            if (m_settings.LineAlignment != 3)
                SetDirty();
        }
        else if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == IDC_COMBO1)
        {
            if (m_settings.CustomRes != (DWORD)SendDlgItemMessage(m_Dlg, IDC_COMBO1, CB_GETCURSEL, 0, 0))
                SetDirty();
        }
        else if (LOWORD(wParam) == IDC_BUTTONRESET)
        {
            LoadDefaults();
            InvalidateRect(GetParent(hwnd), NULL, FALSE);
            SetDirty();
        }
        break;

    // Manage the custom draw buttons
    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case NM_CUSTOMDRAW:
            if (((LPNMHDR)lParam)->idFrom == IDC_BUTTON1)
            {
                LPNMCUSTOMDRAW lpnmCD = (LPNMCUSTOMDRAW)lParam;

                if (lpnmCD->uItemState & CDIS_SELECTED) // Left mouse button clicked on the button
                {
                    DrawCustomButton(lpnmCD, GetSysColor(COLOR_HIGHLIGHT), m_settings.ColorPrimary);

                    return CDRF_DODEFAULT;
                }
                else if (lpnmCD->uItemState & CDIS_HOT) // Mouse is over the button
                {
                    DrawCustomButton(lpnmCD, GetSysColor(COLOR_GRADIENTACTIVECAPTION), m_settings.ColorPrimary);

                    return CDRF_DODEFAULT;
                }
                else
                {
                    DrawCustomButton(lpnmCD, 0x00ACACAC, m_settings.ColorPrimary);

                    return CDRF_DODEFAULT;
                }
            }
            else if (((LPNMHDR)lParam)->idFrom == IDC_BUTTON2)
            {
                LPNMCUSTOMDRAW lpnmCD = (LPNMCUSTOMDRAW)lParam;

                if (lpnmCD->uItemState & CDIS_SELECTED) // Left mouse button clicked on the button
                {
                    DrawCustomButton(lpnmCD, GetSysColor(COLOR_HIGHLIGHT), m_settings.ColorSecondary);

                    return CDRF_DODEFAULT;
                }
                else if (lpnmCD->uItemState & CDIS_HOT) // Mouse is over the button
                {
                    DrawCustomButton(lpnmCD, GetSysColor(COLOR_GRADIENTACTIVECAPTION), m_settings.ColorSecondary);

                    return CDRF_DODEFAULT;
                }
                else
                {
                    DrawCustomButton(lpnmCD, 0x00ACACAC, m_settings.ColorSecondary);

                    return CDRF_DODEFAULT;
                }
            }
            else if (((LPNMHDR)lParam)->idFrom == IDC_BUTTON3)
            {
                LPNMCUSTOMDRAW lpnmCD = (LPNMCUSTOMDRAW)lParam;

                if (lpnmCD->uItemState & CDIS_SELECTED) // Left mouse button clicked on the button
                {
                    DrawCustomButton(lpnmCD, GetSysColor(COLOR_HIGHLIGHT), m_settings.ColorOutline);

                    return CDRF_DODEFAULT;
                }
                else if (lpnmCD->uItemState & CDIS_HOT) // Mouse is over the button
                {
                    DrawCustomButton(lpnmCD, GetSysColor(COLOR_GRADIENTACTIVECAPTION), m_settings.ColorOutline);

                    return CDRF_DODEFAULT;
                }
                else
                {
                    DrawCustomButton(lpnmCD, 0x00ACACAC, m_settings.ColorOutline);

                    return CDRF_DODEFAULT;
                }
            }
            else if (((LPNMHDR)lParam)->idFrom == IDC_BUTTON4)
            {
                LPNMCUSTOMDRAW lpnmCD = (LPNMCUSTOMDRAW)lParam;

                if (lpnmCD->uItemState & CDIS_SELECTED) // Left mouse button clicked on the button
                {
                    DrawCustomButton(lpnmCD, GetSysColor(COLOR_HIGHLIGHT), m_settings.ColorShadow);

                    return CDRF_DODEFAULT;
                }
                else if (lpnmCD->uItemState & CDIS_HOT) // Mouse is over the button
                {
                    DrawCustomButton(lpnmCD, GetSysColor(COLOR_GRADIENTACTIVECAPTION), m_settings.ColorShadow);

                    return CDRF_DODEFAULT;
                }
                else
                {
                    DrawCustomButton(lpnmCD, 0x00ACACAC, m_settings.ColorShadow);

                    return CDRF_DODEFAULT;
                }
            }

            break;
        }

        break;
    } // Switch.

      // Let the parent class handle the message.
    return __super::OnReceiveMessage(hwnd, uMsg, wParam, lParam);
}

CAssFilterStatusProp::CAssFilterStatusProp(LPUNKNOWN pUnk, HRESULT *phr) :
    CBasePropertyPage(NAME("AssFilterStatusProp"), pUnk, IDD_PROPPAGE_STATUS, IDS_PROPPAGE_STATUS_TITLE)
{
}

CAssFilterStatusProp::~CAssFilterStatusProp()
{
}

CUnknown * WINAPI CAssFilterStatusProp::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr)
{
    CAssFilterStatusProp *pNewObject = new CAssFilterStatusProp(pUnk, pHr);
    if (pNewObject == nullptr)
    {
        *pHr = E_OUTOFMEMORY;
    }
    return pNewObject;
}

HRESULT CAssFilterStatusProp::OnConnect(IUnknown *pUnk)
{
    if (pUnk == nullptr)
    {
        return E_POINTER;
    }
    ASSERT(m_pAssFilterSettings == nullptr);
    return pUnk->QueryInterface(&m_pAssFilterSettings);
}

HRESULT CAssFilterStatusProp::OnDisconnect(void)
{
    m_pAssFilterSettings->Release();
    m_pAssFilterSettings = nullptr;

    return S_OK;
}

HRESULT CAssFilterStatusProp::OnActivate(void)
{
    HRESULT hr = S_OK;
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_BAR_CLASSES | ICC_STANDARD_CLASSES;
    if (InitCommonControlsEx(&icc) == FALSE)
    {
        return E_FAIL;
    }

    ASSERT(m_pAssFilterSettings != nullptr);

    const WCHAR *trackname = nullptr;
    const char *tracklang = nullptr;
    const char *subtype = nullptr;

    hr = m_pAssFilterSettings->GetTrackInfo(&trackname, &tracklang, &subtype);
    if (SUCCEEDED(hr))
    {
        SendDlgItemMessage(m_Dlg, IDC_TEST1, WM_SETTEXT, 0, (LPARAM)trackname);
        WCHAR buffer[100];
        _snwprintf_s(buffer, _TRUNCATE, L"%S", tracklang);
        SendDlgItemMessage(m_Dlg, IDC_TEST2, WM_SETTEXT, 0, (LPARAM)buffer);
        _snwprintf_s(buffer, _TRUNCATE, L"%S", subtype);
        SendDlgItemMessage(m_Dlg, IDC_TEST3, WM_SETTEXT, 0, (LPARAM)buffer);
    }

    const WCHAR *consumername = nullptr;
    const WCHAR *consumerversion = nullptr;

    hr = m_pAssFilterSettings->GetConsumerInfo(&consumername, &consumerversion);
    if (SUCCEEDED(hr))
    {
        SendDlgItemMessage(m_Dlg, IDC_TEST4, WM_SETTEXT, 0, (LPARAM)consumername);
        SendDlgItemMessage(m_Dlg, IDC_TEST5, WM_SETTEXT, 0, (LPARAM)consumerversion);
    }

    return hr;
}

HRESULT CAssFilterStatusProp::OnApplyChanges(void)
{
    ASSERT(m_pAssFilterSettings != nullptr);
    HRESULT hr = S_OK;

    return hr;
}

INT_PTR CAssFilterStatusProp::OnReceiveMessage(HWND hwnd,
    UINT uMsg, WPARAM wParam, LPARAM lParam)
{
      // Let the parent class handle the message.
    return __super::OnReceiveMessage(hwnd, uMsg, wParam, lParam);
}

CAssFilterAboutProp::CAssFilterAboutProp(LPUNKNOWN pUnk, HRESULT *phr) :
    CBasePropertyPage(NAME("AssFilterAboutProp"), pUnk, IDD_PROPPAGE_ABOUT, IDS_PROPPAGE_ABOUT_TITLE)
{
}

CAssFilterAboutProp::~CAssFilterAboutProp()
{
}

CUnknown * WINAPI CAssFilterAboutProp::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr)
{
    CAssFilterAboutProp *pNewObject = new CAssFilterAboutProp(pUnk, pHr);
    if (pNewObject == nullptr)
    {
        *pHr = E_OUTOFMEMORY;
    }
    return pNewObject;
}

HRESULT CAssFilterAboutProp::OnConnect(IUnknown *pUnk)
{
    if (pUnk == nullptr)
    {
        return E_POINTER;
    }
    ASSERT(m_pAssFilterSettings == nullptr);
    return pUnk->QueryInterface(&m_pAssFilterSettings);
}

HRESULT CAssFilterAboutProp::OnDisconnect(void)
{
    m_pAssFilterSettings->Release();
    m_pAssFilterSettings = nullptr;

    return S_OK;
}

HRESULT CAssFilterAboutProp::OnActivate(void)
{
    HRESULT hr = S_OK;
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_BAR_CLASSES | ICC_STANDARD_CLASSES;
    if (InitCommonControlsEx(&icc) == FALSE)
    {
        return E_FAIL;
    }

    ASSERT(m_pAssFilterSettings != nullptr);

    HFONT hFont {};
    LOGFONT lf {};
    HWND hwnd = GetDlgItem(NULL, IDC_NAME);
    HDC hdc = GetDC(hwnd);

    lf.lfHeight = -MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    lf.lfWeight = FW_BOLD;
    wcscpy_s(lf.lfFaceName, L"Arial");
    hFont = CreateFontIndirect(&lf);
    ReleaseDC(hwnd, hdc);
    SendDlgItemMessage(m_Dlg, IDC_NAME, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));

#ifdef _WIN64
    const WCHAR *name = TEXT("AssFilterMod (x64)");
#else
    const WCHAR *name = TEXT("AssFilterMod (x32)");
#endif // _WIN64

    SendDlgItemMessage(m_Dlg, IDC_NAME, WM_SETTEXT, 0, (LPARAM)name);

    const WCHAR *version = TEXT("version ") TEXT(ASSF_VERSION_STR) L" (" TEXT(ASSF_HASH_STR) L")";
    SendDlgItemMessage(m_Dlg, IDC_VERSION, WM_SETTEXT, 0, (LPARAM)version);

    return hr;
}

HRESULT CAssFilterAboutProp::OnApplyChanges(void)
{
    ASSERT(m_pAssFilterSettings != nullptr);
    HRESULT hr = S_OK;

    return hr;
}

INT_PTR CAssFilterAboutProp::OnReceiveMessage(HWND hwnd,
    UINT uMsg, WPARAM wParam, LPARAM lParam)
{
      // Let the parent class handle the message.
    return __super::OnReceiveMessage(hwnd, uMsg, wParam, lParam);
}
