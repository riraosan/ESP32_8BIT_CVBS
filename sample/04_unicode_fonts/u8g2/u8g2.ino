

#define LGFX_USE_V1
#include <LovyanGFX.h>
#include <LGFX_8BIT_CVBS.h>
static LGFX_8BIT_CVBS display;
#define M5Canvas LGFX_Sprite

#include "profont.h"
#include "adobex11font.h"

static const lgfx::U8g2font profont10( u8g2_font_profont10_tr );
static const lgfx::U8g2font profont11( u8g2_font_profont11_tr );
static const lgfx::U8g2font profont12( u8g2_font_profont12_tr );
static const lgfx::U8g2font profont15( u8g2_font_profont15_tr );
static const lgfx::U8g2font profont17( u8g2_font_profont17_tr );
static const lgfx::U8g2font profont22( u8g2_font_profont22_tr );
static const lgfx::U8g2font profont29( u8g2_font_profont29_tr );

static const lgfx::U8g2font courB08 ( u8g2_font_courB08_tr );
static const lgfx::U8g2font courB10 ( u8g2_font_courB10_tr );
static const lgfx::U8g2font courB12 ( u8g2_font_courB12_tr );
static const lgfx::U8g2font courB14 ( u8g2_font_courB14_tr );
static const lgfx::U8g2font courB18 ( u8g2_font_courB18_tr );
static const lgfx::U8g2font courB24 ( u8g2_font_courB24_tr );

static const lgfx::U8g2font courR08 ( u8g2_font_courR08_tr );
static const lgfx::U8g2font courR10 ( u8g2_font_courR10_tr );
static const lgfx::U8g2font courR12 ( u8g2_font_courR12_tr );
static const lgfx::U8g2font courR14 ( u8g2_font_courR14_tr );
static const lgfx::U8g2font courR18 ( u8g2_font_courR18_tr );
static const lgfx::U8g2font courR24 ( u8g2_font_courR24_tr );

static const lgfx::U8g2font helvB08 ( u8g2_font_helvB08_tr );
static const lgfx::U8g2font helvB10 ( u8g2_font_helvB10_tr );
static const lgfx::U8g2font helvB12 ( u8g2_font_helvB12_tr );
static const lgfx::U8g2font helvB14 ( u8g2_font_helvB14_tr );
static const lgfx::U8g2font helvB18 ( u8g2_font_helvB18_tr );
static const lgfx::U8g2font helvB24 ( u8g2_font_helvB24_tr );

static const lgfx::U8g2font helvR08 ( u8g2_font_helvR08_tr );
static const lgfx::U8g2font helvR10 ( u8g2_font_helvR10_tr );
static const lgfx::U8g2font helvR12 ( u8g2_font_helvR12_tr );
static const lgfx::U8g2font helvR14 ( u8g2_font_helvR14_tr );
static const lgfx::U8g2font helvR18 ( u8g2_font_helvR18_tr );
static const lgfx::U8g2font helvR24 ( u8g2_font_helvR24_tr );

static const lgfx::U8g2font ncenB08 ( u8g2_font_ncenB08_tr );
static const lgfx::U8g2font ncenB10 ( u8g2_font_ncenB10_tr );
static const lgfx::U8g2font ncenB12 ( u8g2_font_ncenB12_tr );
static const lgfx::U8g2font ncenB14 ( u8g2_font_ncenB14_tr );
static const lgfx::U8g2font ncenB18 ( u8g2_font_ncenB18_tr );
static const lgfx::U8g2font ncenB24 ( u8g2_font_ncenB24_tr );

static const lgfx::U8g2font ncenR08 ( u8g2_font_ncenR08_tr );
static const lgfx::U8g2font ncenR10 ( u8g2_font_ncenR10_tr );
static const lgfx::U8g2font ncenR12 ( u8g2_font_ncenR12_tr );
static const lgfx::U8g2font ncenR14 ( u8g2_font_ncenR14_tr );
static const lgfx::U8g2font ncenR18 ( u8g2_font_ncenR18_tr );
static const lgfx::U8g2font ncenR24 ( u8g2_font_ncenR24_tr );

static const lgfx::U8g2font timB08  ( u8g2_font_timB08_tr  );
static const lgfx::U8g2font timB10  ( u8g2_font_timB10_tr  );
static const lgfx::U8g2font timB12  ( u8g2_font_timB12_tr  );
static const lgfx::U8g2font timB14  ( u8g2_font_timB14_tr  );
static const lgfx::U8g2font timB18  ( u8g2_font_timB18_tr  );
static const lgfx::U8g2font timB24  ( u8g2_font_timB24_tr  );

static const lgfx::U8g2font timR08  ( u8g2_font_timR08_tr  );
static const lgfx::U8g2font timR10  ( u8g2_font_timR10_tr  );
static const lgfx::U8g2font timR12  ( u8g2_font_timR12_tr  );
static const lgfx::U8g2font timR14  ( u8g2_font_timR14_tr  );
static const lgfx::U8g2font timR18  ( u8g2_font_timR18_tr  );
static const lgfx::U8g2font timR24  ( u8g2_font_timR24_tr  );


void setup(){
  display.init();
}

void loop(){
  display.setTextColor(random(0x10000)| 0x8410, random(0x10000)&0x7BEF);

  display.clear();
  display.setCursor(0,0);
  display.setFont(&profont10); display.print("profont10\n");
  display.setFont(&profont11); display.print("profont11\n");
  display.setFont(&profont12); display.print("profont12\n");
  display.setFont(&profont15); display.print("profont15\n");
  display.setFont(&profont17); display.print("profont17\n");
  display.setFont(&profont22); display.print("profont22\n");
  display.setFont(&profont29); display.print("profont29\n");
  delay(2000);
  display.clear();
  display.setCursor(0,0);
  display.setFont(&courB08); display.print("courB08\n");
  display.setFont(&courB10); display.print("courB10\n");
  display.setFont(&courB12); display.print("courB12\n");
  display.setFont(&courB14); display.print("courB14\n");
  display.setFont(&courB18); display.print("courB18\n");
  display.setFont(&courB24); display.print("courB24\n");
  delay(2000);
  display.clear();
  display.setCursor(0,0);
  display.setFont(&courR08); display.print("courR08\n");
  display.setFont(&courR10); display.print("courR10\n");
  display.setFont(&courR12); display.print("courR12\n");
  display.setFont(&courR14); display.print("courR14\n");
  display.setFont(&courR18); display.print("courR18\n");
  display.setFont(&courR24); display.print("courR24\n");
  delay(2000);
  display.clear();
  display.setCursor(0,0);
  display.setFont(&helvB08); display.print("helvB08\n");
  display.setFont(&helvB10); display.print("helvB10\n");
  display.setFont(&helvB12); display.print("helvB12\n");
  display.setFont(&helvB14); display.print("helvB14\n");
  display.setFont(&helvB18); display.print("helvB18\n");
  display.setFont(&helvB24); display.print("helvB24\n");
  delay(2000);
  display.clear();
  display.setCursor(0,0);
  display.setFont(&helvR08); display.print("helvR08\n");
  display.setFont(&helvR10); display.print("helvR10\n");
  display.setFont(&helvR12); display.print("helvR12\n");
  display.setFont(&helvR14); display.print("helvR14\n");
  display.setFont(&helvR18); display.print("helvR18\n");
  display.setFont(&helvR24); display.print("helvR24\n");
  delay(2000);
  display.clear();
  display.setCursor(0,0);
  display.setFont(&ncenB08); display.print("ncenB08\n");
  display.setFont(&ncenB10); display.print("ncenB10\n");
  display.setFont(&ncenB12); display.print("ncenB12\n");
  display.setFont(&ncenB14); display.print("ncenB14\n");
  display.setFont(&ncenB18); display.print("ncenB18\n");
  display.setFont(&ncenB24); display.print("ncenB24\n");
  delay(2000);
  display.clear();
  display.setCursor(0,0);
  display.setFont(&ncenR08); display.print("ncenR08\n");
  display.setFont(&ncenR10); display.print("ncenR10\n");
  display.setFont(&ncenR12); display.print("ncenR12\n");
  display.setFont(&ncenR14); display.print("ncenR14\n");
  display.setFont(&ncenR18); display.print("ncenR18\n");
  display.setFont(&ncenR24); display.print("ncenR24\n");
  delay(2000);
  display.clear();
  display.setCursor(0,0);
  display.setFont(&timB08 ); display.print("timB08\n");
  display.setFont(&timB10 ); display.print("timB10\n");
  display.setFont(&timB12 ); display.print("timB12\n");
  display.setFont(&timB14 ); display.print("timB14\n");
  display.setFont(&timB18 ); display.print("timB18\n");
  display.setFont(&timB24 ); display.print("timB24\n");
  delay(2000);
  display.clear();
  display.setCursor(0,0);
  display.setFont(&timR08 ); display.print("timR08\n");
  display.setFont(&timR10 ); display.print("timR10\n");
  display.setFont(&timR12 ); display.print("timR12\n");
  display.setFont(&timR14 ); display.print("timR14\n");
  display.setFont(&timR18 ); display.print("timR18\n");
  display.setFont(&timR24 ); display.print("timR24\n");
  delay(2000);
}
