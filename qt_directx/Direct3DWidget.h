#ifndef _QD3DWIDGET_H_ 
#define _QD3DWIDGET_H_ 

#include <QtGui/QWidget> 
//#include "ui_Main.h"

struct IDirect3D9; 
struct IDirect3DDevice9; 

class QD3DWidget : public QWidget 
{ 
  Q_OBJECT 

public: 
  /** Constructor */ 
  QD3DWidget( QWidget* pParent = NULL); 

  /** Destructor */ 
  ~QD3DWidget(); 

  /** a hint to Qt to give the widget as much space as possible */ 
  QSizePolicy sizePolicy() const { return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ); } 
  /** a hint to Qt that we take care of the drawing for ourselves, thankyouverymuch */ 
  QPaintEngine *paintEngine() const { return NULL; } 

protected: 
  /** Initialized the D3D environment */ 
  void Setup(); 

  /** Destroys the D3D environment */ 
  void Close(); 

  /** paints the scene */ 
  void paintEvent( QPaintEvent* pEvent); 

private: 
  /** D3D stuff */ 
  IDirect3D9*       mD3D; 
  IDirect3DDevice9* mDevice; 
  //Ui::Form ui;
}; 

#endif // _QD3DWIDGET_H_ 