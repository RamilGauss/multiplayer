#include "input.h"

#define MessageBox MessageBoxA

bool Input::InitInput(WId w)
{

    ZeroMemory(&DIks, sizeof(BYTE) * 256);
    for(unsigned short i = 0; i < 256; i++)
        kNew[i] = true;
    ZeroMemory(&DIms, sizeof(DIMOUSESTATE2));
    for(int i = 0; i < 8; i++)
        mbNew[i] = true;
    if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpDI, NULL)))
    {
        MessageBox(NULL, "Не могу создать DirectInput!", "ERROR!!!", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }
    if(FAILED(lpDI->CreateDevice(GUID_SysKeyboard, &lpDIKeyboard, NULL)))
    {
        MessageBox(NULL,"Не могу создать интерфейс для клавиатуры!","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }
    if(FAILED(lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard)))
    {
        MessageBox(NULL,"Формат клавиатуры не описан!","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }
    if(FAILED(lpDIKeyboard->SetCooperativeLevel(w, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE )))
    {
        MessageBox(NULL,"Ввод для клавиатуры не установлен!","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }
    if(FAILED(lpDI->CreateDevice(GUID_SysMouse, &lpDIMouse, NULL)))
    {
        MessageBox(NULL,"Не могу создать интерфейс для мыши!","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }
    if(FAILED(lpDIMouse->SetDataFormat(&c_dfDIMouse2)))
    {
        MessageBox(NULL,"Формат мыши не описан!","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }
    if(FAILED(lpDIMouse->SetCooperativeLevel(w, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
    {
        MessageBox(NULL,"Ввод для мыши не установлен!","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }
    return true;
}

void Input::Update()
{
    HRESULT hr;
    hr = lpDIKeyboard->GetDeviceState(sizeof(BYTE) * 256, &DIks);
    if(FAILED(hr))
    {
        do 
        {
            hr = lpDIKeyboard->Acquire();
        } while(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED);
    }
    hr = lpDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &DIms);
    if(FAILED(hr))
    {
        do 
        {
            hr = lpDIMouse->Acquire();
        } while(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED);
    }
}

float Input::GetMouseSpeedX()
{
    return (float)DIms.lX;
}

float Input::GetMouseSpeedY()
{
    return (float)DIms.lY;
}

float Input::GetMouseSpeedZ()
{
    return (float)DIms.lZ;
}

bool Input::isKey(BYTE in_kb)
{
    if(DIks[in_kb] & 0x80)
        return true;
    return false;
}

bool Input::isKeyNew(BYTE in_kb)
{
    if(DIks[in_kb] & 0x80) 
    {
        if(kNew[in_kb])
        {
            kNew[in_kb] = false;
            return true;
        }
    }
    else
        kNew[in_kb] = true;
    return false;
}

bool Input::isMBut(BYTE in_mb)
{
    if(DIms.rgbButtons[in_mb] & 0x80)
        return true;
    else
        return false;
}

bool Input::isMButNew(BYTE in_mb)
{
    if(DIms.rgbButtons[in_mb] & 0x80)
    {
        if(mbNew[in_mb])
        {
            mbNew[in_mb] = false;
            return true;
        }
    }
    else
        mbNew[in_mb] = true;
    return false;
}

void Input::Shutdown()
{
    if(lpDIKeyboard)
        lpDIKeyboard->Unacquire();
    if(lpDIMouse)
        lpDIMouse->Unacquire();

    if(lpDIKeyboard)
        lpDIKeyboard->Release();
    if(lpDIMouse)
        lpDIMouse->Release();

    if(lpDI)
        lpDI->Release();
}