/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __ERROR_H__
#define __ERROR_H__

// Danh sach cac loi trong qua trinh phan tich tu vung
typedef enum {
  ERR_ENDOFCOMMENT, // (* comment
  ERR_IDENTTOOLONG, // Charrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  ERR_INVALIDCHARCONSTANT, // ' A'
  ERR_INVALIDSYMBOL, // !!!
  ERR_NUMBERTOOLONG // 999999999999999, max 10 chu so 
} ErrorCode;

// Cac thong bao loi
#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"
#define ERM_NUMBERTOOLONG "Number too long!"

// Ham thong bao loi
void error(ErrorCode err, int lineNo, int colNo);

#endif

