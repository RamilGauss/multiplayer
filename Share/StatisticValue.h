/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef StatisticValueH
#define StatisticValueH

#include <math.h>
             
//---------------------------------------------------------------------------
// ������ ������������� TStatXXX ��� ������� �������������� ����������
// (���. ��������, ���������, ���) �� ����������� ������ ������.
// ��� ������ �� ������������ ������������� ������ ����� ������� ���������� �� 
// ��������� ���-�� ��������� ��������. ������ ���������� �������, 
// � ����� �������� ������, ��������� ����������� � ��������.
// ������� �� ������������� ������ �������� ������ ������������� � �� ����� 
// ������������� ��������.
//---------------------------------------------------------------------------
inline double calcSKO( int sum, int sum2, int count )
{
  // � ��������:
  // disp = ( mSum2 - 2*(mSum/mCount)*mSum + ((mSum*mSum)/mCount) ) / mCount )
  // ��������:
  double disp = ( sum2 - sum*sum/(double)count ) / count;
  //if(disp<0) disp = 0;//-disp;//???

  return sqrt( disp );
}
//---------------------------------------------------------------------------
inline double calcSKO( unsigned __int64 sum, unsigned __int64 sum2, int count )
{
  // � ��������:
  // disp = ( mSum2 - 2*(mSum/mCount)*mSum + ((mSum*mSum)/mCount) ) / mCount )
  // ��������:
  double disp = ( sum2 - sum*sum/(double)count ) / count;
  //if(disp<0) disp = 0;//-disp;//???

  return sqrt( disp );
}
//---------------------------------------------------------------------------
inline double calcSKO( double sum, double sum2, int count )
{
  // � ��������:
  // disp = ( mSum2 - 2*(mSum/mCount)*mSum + ((mSum*mSum)/mCount) ) / mCount )
  // ��������:
  double disp = ( sum2 - sum*sum/(double)count ) / count;

  return sqrt( disp );
}
//---------------------------------------------------------------------------
// ������ ������� ����������� ������� 
#define TStatType_char    TStatValueT<char,   float, unsigned char>
#define TStatType_short   TStatValueT<short,  float, unsigned char>
#define TStatType_int     TStatValueT<int,    double,unsigned char>
#define TStatType_double  TStatValueT<double, double,unsigned char>

//---------------------------------------------------------------------------
// ������ �������������� ���������� ��� ��������� ������ ��������
// TValue, TAverage, TCount - ���� ������ ��� �������� ��������������� �������
template <typename TValue, typename TAverage, typename TCount>
class TStatValueT
{
protected:  
  TAverage mAverage;  // ������� �������� (���. ��������)
  mutable float mSKO; // ������������������ ���������� (���)

  TCount  mMaxCount;  // ���-�� ��������� �������� �� ������� ������� ���������� ����������
  TCount  mCount;     // ���-�� ��������� ���������� ����������
  double  mSum;       // ����� ��������
  double  mSum2;      // ����� ��������� ��������

public:  

  TStatValueT( TCount maxCount )
  {
    mMaxCount = maxCount;
    reset();
  }

  // �������� �� ������
  bool empty() const
  {
    return mCount == 0;
  }

  // ���������� ��������� ��������
  void reset()
  {
    mSKO = 0;
    mCount = 0;
    mSum = mSum2 = 0;
    mAverage = 0;
  }
  
  // ���������� ��������� ��������
  void reset( TValue value )
  {
    mCount = 1;
    mSum = value;
    mSum2 = value*value;

    mAverage = (TAverage)value;
    mSKO = 0;
  }

  // �������� � ����� ��������� �����
  void add( TValue value )
  {
    mSum += value;
    mSum2 += value*value;
    mCount++;
    mAverage = (TAverage)( mSum / mCount );
    
    if( mCount >= mMaxCount )
    {
      mSum  -= mAverage;
      mSum2 -= mSum2/mCount;
      mCount--;
    }
    mSKO = 0;
  }


  TAverage summa() const
  {
    return TAverage(mSum);
  }
  // ������� �������������� �������� (���. ��������)
  TAverage aver() const
  {
    return mAverage;
  }

  // ������������������ ���������� (���)
  float getSKO() const
  {
    if( (int)mSKO == 0 && mCount )
      mSKO = (float)calcSKO( mSum, mSum2, mCount );
    return mSKO;
  }

  //bool save( TIOStream& od ) const 
  //{
  //  return 
  //    od.put( mCount) &&
  //    od.put( mSum  ) &&
  //    od.put( mSum2 );
  //}

  //bool load( TIOStream& od )
  //{
  //  if( od.get( mCount) &&
  //      od.get( mSum  ) &&
  //      od.get( mSum2 ) )
  //  {
  //    mAverage = mCount ? mSum/mCount : 0;
  //    mSKO = 0;
  //    return true;
  //  }
  //  return false;
  //}

	// ���-�� ��������� ���������� ����������
	TCount GetCount()	    { return mCount; }

	// ��� ���-�� ��������� ���������� ����������
	TCount GetMaxCount()	{ return mMaxCount; }
	
	void SetMaxCount( TCount cnt )
	{
		mMaxCount = cnt;
		if( mCount >= mMaxCount )
		{
			int delta = mCount - mMaxCount + 1;
			mSum -= mAverage*delta;
			mSum2 -= (mSum2/mCount)*delta;
		}
	}
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ������ �������������� ���������� ��� ������������ ������ �������� (�������� - ����)
// ����� ������� �������� �� ���������: 
//  4 | 1
//  -----
//  3 | 2
// �������������� ���������� ���������� ��������� �������� �� 4 � 1 ���������.

// ������ ������� ����������� ������� 
#define TStatCyclicType_char(maxValue)    TStatCyclicValueT<char,   float,  guchar, maxValue>
#define TStatCyclicType_short(maxValue)   TStatCyclicValueT<short,  float,  guchar, maxValue>
#define TStatCyclicType_int(maxValue)     TStatCyclicValueT<int,    double, guchar, maxValue>
#define TStatCyclicType_double(maxValue)  TStatCyclicValueT<double, double, guchar, maxValue>

// TValue, TAverage, TCount - ���� ������ ��� �������� ��������������� �������
template <typename TValue, typename TAverage, typename TCount, int maxValue >
class TStatCyclicValueT
{
protected:  
  char      mHasHiValue;// ������� � ������ �������� �� 4 ��������
  TAverage  mAverage;   // ������� �������� (���. ��������)
  mutable float mSKO; // ������������������ ���������� (���)

  TCount  mMaxCount;  // ���-�� ��������� �������� �� ������� ������� ���������� ����������
  TCount  mCount;     // ���-�� ��������� ���������� ����������
  double  mSum;       // ����� ��������
  double  mSum2;      // ����� ��������� ��������

public:  

  TStatCyclicValueT( TCount maxCount )
  {
    reset();
    mMaxCount = maxCount;
  }

  // �������� �� ������
  bool empty() const
  {
    return mCount == 0;
  }

  // ����� � ��������� ���������
  void reset()
  {
    mHasHiValue = false;
    mSKO = 0;
    mCount = 0;
    mSum = mSum2 = 0;
    mAverage = 0;
  }

  // ���������� ��������� ��������
  void reset( TValue value )
  {
    mHasHiValue = ( value > maxValue*3/4 ) ? true  : false;

    mCount = 1;
    mSum = value;
    mSum2 = value*value;

    mAverage = (TAverage)value;
    mSKO = 0;
  }

  // �������� � ����� ��������� �����
  void add( TValue value )
  {
    // ��������� �������� ��� ����� �� ���� "��������" ��������
    if( mCount == 0 )
    {
      if( value > maxValue*3/4 )
        mHasHiValue = true;
    }
    else 
    {
      if( mHasHiValue ) 
      {
        if( value < maxValue/4 )
          value = maxValue + value;
      }
      else
      {
        if( value > maxValue*3/4 )
        {
          mHasHiValue = true;
          if( mAverage < maxValue/4 )
          {
            // ��������� ��� ���������� �������� �� 1 �������� � 5:
            mAverage += maxValue;
            getSKO();
            // �������� ������ �������� ������������� �� ���������� ���
            // 1)  disp = sko*sko;
            // 2)  disp = ( sum2 - sum*sum/(double)count ) / count;
            //     sum2 = disp * count + sum*sum/(double)count;
            //               
            mSum  = mAverage * mCount;
            mSum2 = mSKO*mSKO*mCount + mSum*mSum/mCount;
          }
        }
      }
    }
    mSum  += value;
    mSum2 += value*value;
    mCount++;
    mAverage = (TAverage)( mSum / mCount );

    if( mCount >= mMaxCount )
    {
      mSum -= mAverage;
      mSum2 -= mSum2/mCount;
      mCount = mMaxCount - 1;
    }
    mSKO = 0;
  }

  // ������� �������������� �������� (���. ��������)
  TAverage aver() const
  {
    return mAverage >= maxValue ? mAverage - maxValue : mAverage;
  }

  // ������������������ ���������� (���)
  float getSKO() const
  {
    if( mSKO == 0 && mCount)
      mSKO = (float)calcSKO( mSum, mSum2, mCount );
    return mSKO;
  }

  //bool save( TIOStream& od ) const 
  //{
  //  return 
  //    od.put( *(signed char*)&mHasHiValue) &&
  //    od.put( mCount) &&
  //    od.put( mSum  ) &&
  //    od.put( mSum2 );
  //}

  //bool load( TIOStream& od )
  //{
  //  if( od.get( *(signed char*)&mHasHiValue ) &&
  //      od.get( mCount) &&
  //      od.get( mSum  ) &&
  //      od.get( mSum2 ) )
  //  {
  //    mSKO = 0;
  //    mAverage = (TAverage)(mCount ? mSum/mCount : 0);
  //    return true;
  //  }
  //  return false;
  //}

  // ���-�� ��������� ���������� ����������
  TCount GetCount()	{ return mCount; }

  // ��� ���-�� ��������� ���������� ����������
  TCount GetMaxCount()	{ return mMaxCount; }

  void SetMaxCount(TCount cnt)
  {
    mMaxCount = cnt;
    if( mCount >= mMaxCount )
    {
      int delta = mCount - mMaxCount + 1;
      mSum -= mAverage*delta;
      mSum2 -= (mSum2/mCount)*delta;
    }
  }
};
//---------------------------------------------------------------------------
// ������ �������������� ���������� ��� ��������� ������ �������� ������������ �������� ��������
// TValue, TAverage, TSKO, TCount - ���� ������ ��� �������� ��������������� �������
template <typename TValue, typename TCount>
class TAbsStatValueT
{
protected:  
  TValue  mAbsValue;  // ������� ��������
  mutable float mSKO; // ������������������ ���������� (���)

  TCount  mMaxCount;// ���-�� ��������� �������� �� ������� ������� ���������� ����������
  TCount  mCount;   // ���-�� ��������� ���������� ����������
  double  mSum2;    // ����� ��������� ��������

public:  

  TAbsStatValueT( TCount maxCount, TValue absValue )
  {
    mSKO = 0;
    mCount = 0;
    mMaxCount = maxCount;
    mSum2 = 0;
    mAbsValue = absValue;
  }

  // �������� �� ������
  bool empty() const
  {
    return mCount == 0;
  }

  // ���������� ��������� ��������
  void reset( TValue value )
  {
    mCount = 1;
    mSum2 = ( value - mAbsValue ) * ( value - mAbsValue );  
    mSKO = 0;
  }

  // �������� � ����� ��������� �����
  void add( TValue value )
  {
    mSum2 += ( value - mAbsValue ) * ( value - mAbsValue );  

    if( ++mCount >= mMaxCount )
    {
      mSum2 -= mSum2/mCount;
      mCount = mMaxCount - 1;
    }
    mSKO = 0;
  }

  float getSKO()
  {
    if( mSKO == 0 && mCount )
    {
      double disp = mSum2 / mCount;
      mSKO = (float)sqrt( disp );
    }
    return mSKO;
  }

  // ���-�� ��������� ���������� ����������
  TCount GetCount()	    { return mCount; }

  // ��� ���-�� ��������� ���������� ����������
  TCount GetMaxCount()	{ return mMaxCount; }

  void SetMaxCount(TCount cnt)
  {
    mMaxCount = cnt;
    if( mCount >= mMaxCount )
    {
      int delta = mCount - mMaxCount + 1;
      mSum2 -= (mSum2/mCount)*delta;
    }
  }
};
//---------------------------------------------------------------------------

#endif
