/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef TObjectH
#define TObjectH

#include "BL_IOStream.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

DECLARE_CLASS( Object )

// ������� ����� ��������
class GBASELIB_EI TObject
{
public:
  TObject() {}
  TObject( const TObject& ) {}
  virtual ~TObject() {}

  // �������� ���� �� ��������� ������� <obj>
  virtual bool operator==( const TObject& ) const
    { return false; }

  // ��������� ������ � �����
  virtual bool save( TIOStream& ) const
    { return true; }

  // ��������� ������ �� ������
  virtual bool load( TIOStream& )
    { return true; }

  // ��������� ������ ����������� ����� �������
  // ���������: ���-�� ����
  virtual int length() const
    { return 0; }

  // ������� ��������� ������� ������ ����
  virtual PObject dubl() const
    { return new TObject (); }

  // ����������� � ���� ������ <obj>
  virtual bool Assign( const TObject& )
    { return true; }

};

#endif
