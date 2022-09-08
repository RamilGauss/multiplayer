#ifndef TypeTankH
#define TypeTankH



namespace nsTank_Type
{
  enum{
    eCntTypeTank        = 5,
    eTypeTankLight      = 0,
    eTypeTankMedium     = (0xFFFF/eCntTypeTank),
    eTypeTankHeavy      = ((0xFFFF*2)/eCntTypeTank),
    eTypeTankPT         = ((0xFFFF*3)/eCntTypeTank),
    eTypeTankMediumSAU  = ((0xFFFF*4)/eCntTypeTank),
  };
};
//--------------------------------------------------------------
namespace nsTank_ID
{
  enum{
    eID_T_34    = nsTank_Type::eTypeTankMedium,
    eID_Pz_34   = nsTank_Type::eTypeTankMedium+1,

    eID_IS      = nsTank_Type::eTypeTankHeavy,
    eID_Tiger   = nsTank_Type::eTypeTankHeavy+1,
  };
  
};
//--------------------------------------------------------------

namespace nsStrTank
{
  extern const char* strTank_T_34;
  extern const char* strTank_T_34_Ini;
  extern const char* strTank_IS;
  extern const char* strTank_IS_Ini;
  extern const char* strTank_Tiger;
  extern const char* strTank_Tiger_Ini;
}



#endif