
#include <gl\gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <string>

namespace freetype {

// Inside Of This Namespace, Give Ourselves The Ability
// To Write Just "vector" Instead Of "std::vector"
using std::vector;
 
// Ditto For String.
using std::string;
 
// This Holds All Of The Information Related To Any
// FreeType Font That We Want To Create. 
struct font_data {
    float h;                                        // Holds The Height Of The Font.
    GLuint * textures;                                  // Holds The Texture Id's
    GLuint list_base;                                   // Holds The First Display List Id
 
    // The Init Function Will Create A Font With
    // The Height h From The File fname.
    void init(const char * fname, unsigned int h);
 
    // Free All The Resources Associated With The Font.
        void clean();
};

void print(const font_data &ft_font, float x, float y, const char *fmt, ...){

}

inline int next_p2 (int a )
{
    int rval=1;
    // rval<<=1 Is A Prettier Way Of Writing rval*=2;
    while(rval<a) rval<<=1;
    return rval;
}


   //prime freetype font
    FT_Library    ft;
    FT_Face       face;

    FT_UInt index;

  #if __APPLE__
    fp = "/System/Library/Fonts/Supplemental/Times New Roman.ttf";
  #endif

  #if _WIN64
    const char* fp = "Courier.ttf";
  #endif


FT_Init_FreeType(&ft);
    // font
    FT_Error ret = FT_New_Face(ft, "./Courier.ttf", 0, &face);
     if (ret){
      std::puts(FT_Error_String(ret));
      return 1; 
    }
    
    FT_Set_Pixel_Sizes(face, 0, 36);

    FT_GlyphSlot g = face->glyph;
    int w = 0;
    int h = 0;
    
    for (unsigned char i = 0; i < 128; i++){
      index = FT_Get_Char_Index(face, i);
      //printf("LOADED INDEX: %c \n", index);
      FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);
      FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
      glNewList(base + i, GL_COMPILE);  // we want to start @ 0
      glBitmap(face->glyph->bitmap.width,
          face->glyph->bitmap.rows,
          0.0f,
          0.0f,
          (float)(face->glyph->bitmap.width +2),
          0.0f,
          face->glyph->bitmap.buffer);
      glEndList();
    }
    //once loaded, unload fonts / cleanup

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
