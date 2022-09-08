#ifndef _INPUT_H_
#define _INPUT_H_

#include <qt/qwidget.h>
#include <dinput.h>

class Input
{
public:
    bool InitInput(WId w);
    void Update(void);
    void Shutdown(void);
    
    bool isKey(BYTE in_kb);
    bool isKeyNew(BYTE in_kb);
    bool isMBut(BYTE in_mb);
    bool isMButNew(BYTE in_mb);

    float GetMouseSpeedX(void);
    float GetMouseSpeedY(void);
    float GetMouseSpeedZ(void);
private:
    LPDIRECTINPUT8 lpDI;
    LPDIRECTINPUTDEVICE8 lpDIKeyboard;
    LPDIRECTINPUTDEVICE8 lpDIMouse;
    BYTE DIks[256];
    bool kNew[256];
    DIMOUSESTATE2 DIms;
    bool mbNew[8];
};

#endif