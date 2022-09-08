#ifndef BoundingBoxH
#define BoundingBoxH


#if defined( WIN32 )
#pragma pack(push, 1)
#endif


class TBoundingBox
{

public:

  struct TPoint3
  {
    float x;
    float y;
    float z;
  };

protected:
  TPoint3 mMin;
  TPoint3 mMax;

public:


  TBoundingBox();
  ~TBoundingBox();

  void SetBound(TPoint3& min,TPoint3& max);
  void AddBound(TPoint3& min,TPoint3& max);

  TPoint3& Min(){return mMin;};
  TPoint3& Max(){return mMax;};
};

#if defined( WIN32 )
#pragma pack(pop)
#endif


#endif