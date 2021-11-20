

const   char   string5X7Code[]= {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //  
0x20,0x20,0x20,0x20,0x00,0x00,0x20,0x00, // !
0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00, // "
0x50,0x50,0xf8,0x50,0xf8,0x50,0x50,0x00, // #
0x20,0x78,0xa0,0x70,0x28,0xf0,0x20,0x00, // $
0xc0,0xc8,0x10,0x20,0x40,0x98,0x18,0x00, // %
0x60,0x90,0xa0,0x40,0xa8,0x90,0x68,0x00, // &
0x60,0x20,0x60,0x00,0x00,0x00,0x00,0x00, // '
0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00, // (
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00, // )
0x00,0x20,0xa8,0x70,0xa8,0x20,0x00,0x00, // *
0x00,0x20,0x20,0xf8,0x20,0x20,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x60,0x20,0x40,0x00, // ,
0x00,0x00,0x00,0xf8,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00, // .
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00, // /
0x70,0x88,0x98,0xa8,0xc8,0x88,0x70,0x00, // 0
0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00, // 1
0x70,0x88,0x08,0x10,0x20,0x40,0xf8,0x00, // 2
0xf8,0x10,0x20,0x10,0x08,0x88,0x70,0x00, // 3
0x10,0x30,0x50,0x90,0xf8,0x10,0x10,0x00, // 4
0xf8,0x80,0xf0,0x08,0x08,0x88,0x70,0x00, // 5
0x30,0x40,0x80,0xf0,0x88,0x88,0x70,0x00, // 6
0xf8,0x08,0x10,0x20,0x40,0x40,0x40,0x00, // 7
0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00, // 8
0x70,0x88,0x88,0x78,0x08,0x10,0x60,0x00, // 9
0x00,0x60,0x60,0x00,0x60,0x60,0x00,0x00, // :
0x00,0x60,0x60,0x00,0x60,0x20,0x40,0x00, // ;
0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x00, // <
0x00,0x00,0xf8,0x00,0xf8,0x00,0x00,0x00, // =
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00, // >
0x70,0x88,0x08,0x10,0x20,0x00,0x20,0x00, // ?
0x70,0x88,0x08,0x68,0xa8,0xa8,0x70,0x00, // @
0x70,0x88,0x88,0x88,0xf8,0x88,0x88,0x00, // A
0xf0,0x88,0x88,0xf0,0x88,0x88,0xf0,0x00, // B
0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00, // C
0xe0,0x90,0x88,0x88,0x88,0x90,0xe0,0x00, // D
0xf8,0x80,0x80,0xf0,0x80,0x80,0xf8,0x00, // E
0xf8,0x80,0x80,0xf0,0x80,0x80,0x80,0x00, // F
0x70,0x88,0x80,0xb8,0x88,0x88,0x78,0x00, // G
0x88,0x88,0x88,0xf8,0x88,0x88,0x88,0x00, // H
0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // I
0x38,0x10,0x10,0x10,0x10,0x90,0x60,0x00, // J
0x88,0x90,0xa0,0xc0,0xa0,0x90,0x88,0x00, // K
0x80,0x80,0x80,0x80,0x80,0x80,0xf8,0x00, // L
0x88,0xd8,0xa8,0xa8,0x88,0x88,0x88,0x00, // M
0x88,0x88,0xc8,0xa8,0x98,0x88,0x88,0x00, // N
0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // O
0xf0,0x88,0x88,0xf0,0x80,0x80,0x80,0x00, // P
0x70,0x88,0x88,0x88,0xa8,0x90,0x68,0x00, // Q
0xf0,0x88,0x88,0xf0,0xa0,0x90,0x88,0x00, // R
0x78,0x80,0x80,0x70,0x08,0x08,0xf0,0x00, // S
0xf8,0x20,0x20,0x20,0x20,0x20,0x20,0x00, // T
0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // U
0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00, // V
0x88,0x88,0x88,0xa8,0xa8,0xa8,0x50,0x00, // W
0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00, // X
0x88,0x88,0x88,0x50,0x20,0x20,0x20,0x00, // Y
0xf8,0x08,0x10,0x20,0x40,0x80,0xf8,0x00, // Z
0x70,0x40,0x40,0x40,0x40,0x40,0x70,0x00, // [
0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00, // \
0x70,0x10,0x10,0x10,0x10,0x10,0x70,0x00, // ]
0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0x00, // _
0x80,0x40,0x20,0x00,0x00,0x00,0x00,0x00, // `
0x00,0x00,0x70,0x08,0x78,0x88,0x78,0x00, // a
0x80,0x80,0x80,0xf0,0x88,0x88,0xf0,0x00, // b
0x00,0x00,0x78,0x80,0x80,0x80,0x78,0x00, // c
0x08,0x08,0x08,0x78,0x88,0x88,0x78,0x00, // d
0x00,0x00,0x70,0x88,0xf8,0x80,0x78,0x00, // e
0x10,0x28,0x20,0x70,0x20,0x20,0x20,0x00, // f
0x00,0x00,0x78,0x88,0x78,0x08,0xf0,0x00, // g
0x80,0x80,0x80,0xf0,0x88,0x88,0x88,0x00, // h
0x00,0x20,0x00,0x20,0x20,0x20,0x20,0x00, // i
0x10,0x00,0x10,0x10,0x10,0x90,0x60,0x00, // j
0x80,0x80,0x90,0xa0,0xc0,0xa0,0x90,0x00, // k
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // l
0x00,0x00,0xd8,0xa8,0xa8,0xa8,0x88,0x00, // m
0x00,0x00,0xb0,0xc8,0x88,0x88,0x88,0x00, // n
0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00, // o
0x00,0x00,0xf0,0x88,0xf0,0x80,0x80,0x00, // p
0x00,0x00,0x78,0x88,0x78,0x08,0x08,0x00, // q
0x00,0x00,0xb0,0xc8,0x80,0x80,0x80,0x00, // r
0x00,0x00,0x78,0x80,0x70,0x08,0xf0,0x00, // s
0x20,0x20,0xf8,0x20,0x20,0x28,0x10,0x00, // t
0x00,0x00,0x88,0x88,0x88,0x88,0x70,0x00, // u
0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00, // v
0x00,0x00,0x88,0x88,0xa8,0xa8,0x50,0x00, // w
0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00, // x
0x00,0x00,0x88,0x50,0x20,0x20,0x40,0x00, // y
0x00,0x00,0xf8,0x10,0x20,0x40,0xf8,0x00, // z
0x30,0x40,0x40,0x80,0x40,0x40,0x30,0x00, // {
0x20,0x20,0x20,0x00,0x20,0x20,0x20,0x00, // |
0x60,0x10,0x10,0x08,0x10,0x10,0x60,0x00, // }
0x08,0x70,0x80,0x00,0x00,0x00,0x00,0x00, // ~
0x00,0x0b,0x00,0x20,0x01,0x01,0x00,0x08 //
};



const   char   string7X8Code[]= {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //  
0x30,0x78,0x78,0x30,0x30,0x00,0x30,0x00, // !
0x6c,0x6c,0x6c,0x00,0x00,0x00,0x00,0x00, // "
0x6c,0x6c,0xfe,0x6c,0xfe,0x6c,0x6c,0x00, // #
0x30,0x7c,0xc0,0x78,0x0c,0xf8,0x30,0x00, // $
0x00,0xc6,0xcc,0x18,0x30,0x66,0xc6,0x00, // %
0x38,0x6c,0x38,0x76,0xdc,0xcc,0x76,0x00, // &
0x60,0x60,0xc0,0x00,0x00,0x00,0x00,0x00, // '
0x18,0x30,0x60,0x60,0x60,0x30,0x18,0x00, // (
0x60,0x30,0x18,0x18,0x18,0x30,0x60,0x00, // )
0x00,0x66,0x3c,0xfe,0x3c,0x66,0x00,0x00, // *
0x00,0x30,0x30,0xfc,0x30,0x30,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x60, // ,
0x00,0x00,0x00,0xfc,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00, // .
0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00, // /
0x7c,0xc6,0xce,0xde,0xf6,0xe6,0x7c,0x00, // 0
0x30,0x70,0x30,0x30,0x30,0x30,0xfc,0x00, // 1
0x78,0xcc,0x0c,0x38,0x60,0xcc,0xfc,0x00, // 2
0x78,0xcc,0x0c,0x38,0x0c,0xcc,0x78,0x00, // 3
0x1c,0x3c,0x6c,0xcc,0xfe,0x0c,0x1e,0x00, // 4
0xfc,0xc0,0xf8,0x0c,0x0c,0xcc,0x78,0x00, // 5
0x38,0x60,0xc0,0xf8,0xcc,0xcc,0x78,0x00, // 6
0xfc,0xcc,0x0c,0x18,0x30,0x30,0x30,0x00, // 7
0x78,0xcc,0xcc,0x78,0xcc,0xcc,0x78,0x00, // 8
0x78,0xcc,0xcc,0x7c,0x0c,0x18,0x70,0x00, // 9
0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00, // :
0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x60, // ;
0x18,0x30,0x60,0xc0,0x60,0x30,0x18,0x00, // <
0x00,0x00,0xfc,0x00,0x00,0xfc,0x00,0x00, // =
0x60,0x30,0x18,0x0c,0x18,0x30,0x60,0x00, // >
0x78,0xcc,0x0c,0x18,0x30,0x00,0x30,0x00, // ?
0x7c,0xc6,0xde,0xde,0xde,0xc0,0x78,0x00, // @
0x30,0x78,0xcc,0xcc,0xfc,0xcc,0xcc,0x00, // A
0xfc,0x66,0x66,0x7c,0x66,0x66,0xfc,0x00, // B
0x3c,0x66,0xc0,0xc0,0xc0,0x66,0x3c,0x00, // C
0xf8,0x6c,0x66,0x66,0x66,0x6c,0xf8,0x00, // D
0xfe,0x62,0x68,0x78,0x68,0x62,0xfe,0x00, // E
0xfe,0x62,0x68,0x78,0x68,0x60,0xf0,0x00, // F
0x3c,0x66,0xc0,0xc0,0xce,0x66,0x3e,0x00, // G
0xcc,0xcc,0xcc,0xfc,0xcc,0xcc,0xcc,0x00, // H
0x78,0x30,0x30,0x30,0x30,0x30,0x78,0x00, // I
0x1e,0x0c,0x0c,0x0c,0xcc,0xcc,0x78,0x00, // J
0xe6,0x66,0x6c,0x78,0x6c,0x66,0xe6,0x00, // K
0xf0,0x60,0x60,0x60,0x62,0x66,0xfe,0x00, // L
0xc6,0xee,0xfe,0xfe,0xd6,0xc6,0xc6,0x00, // M
0xc6,0xe6,0xf6,0xde,0xce,0xc6,0xc6,0x00, // N
0x38,0x6c,0xc6,0xc6,0xc6,0x6c,0x38,0x00, // O
0xfc,0x66,0x66,0x7c,0x60,0x60,0xf0,0x00, // P
0x78,0xcc,0xcc,0xcc,0xdc,0x78,0x1c,0x00, // Q
0xfc,0x66,0x66,0x7c,0x6c,0x66,0xe6,0x00, // R
0x78,0xcc,0xe0,0x70,0x1c,0xcc,0x78,0x00, // S
0xfc,0xb4,0x30,0x30,0x30,0x30,0x78,0x00, // T
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xfc,0x00, // U
0xcc,0xcc,0xcc,0xcc,0xcc,0x78,0x30,0x00, // V
0xc6,0xc6,0xc6,0xd6,0xfe,0xee,0xc6,0x00, // W
0xc6,0xc6,0x6c,0x38,0x38,0x6c,0xc6,0x00, // X
0xcc,0xcc,0xcc,0x78,0x30,0x30,0x78,0x00, // Y
0xfe,0xc6,0x8c,0x18,0x32,0x66,0xfe,0x00, // Z
0x78,0x60,0x60,0x60,0x60,0x60,0x78,0x00, // [
0xc0,0x60,0x30,0x18,0x0c,0x06,0x02,0x00, // \
0x78,0x18,0x18,0x18,0x18,0x18,0x78,0x00, // ]
0x10,0x38,0x6c,0xc6,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe, // _
0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00, // `
0x00,0x00,0x78,0x0c,0x7c,0xcc,0x76,0x00, // a
0xe0,0x60,0x60,0x7c,0x66,0x66,0xdc,0x00, // b
0x00,0x00,0x78,0xcc,0xc0,0xcc,0x78,0x00, // c
0x1c,0x0c,0x0c,0x7c,0xcc,0xcc,0x76,0x00, // d
0x00,0x00,0x78,0xcc,0xfc,0xc0,0x78,0x00, // e
0x38,0x6c,0x60,0xf0,0x60,0x60,0xf0,0x00, // f
0x00,0x00,0x76,0xcc,0xcc,0x7c,0x0c,0xf8, // g
0xe0,0x60,0x6c,0x76,0x66,0x66,0xe6,0x00, // h
0x30,0x00,0x70,0x30,0x30,0x30,0x78,0x00, // i
0x0c,0x00,0x0c,0x0c,0x0c,0xcc,0xcc,0x78, // j
0xe0,0x60,0x66,0x6c,0x78,0x6c,0xe6,0x00, // k
0x70,0x30,0x30,0x30,0x30,0x30,0x78,0x00, // l
0x00,0x00,0xcc,0xfe,0xfe,0xd6,0xc6,0x00, // m
0x00,0x00,0xf8,0xcc,0xcc,0xcc,0xcc,0x00, // n
0x00,0x00,0x78,0xcc,0xcc,0xcc,0x78,0x00, // o
0x00,0x00,0xdc,0x66,0x66,0x7c,0x60,0xf0, // p
0x00,0x00,0x76,0xcc,0xcc,0x7c,0x0c,0x1e, // q
0x00,0x00,0xdc,0x76,0x66,0x60,0xf0,0x00, // r
0x00,0x00,0x7c,0xc0,0x78,0x0c,0xf8,0x00, // s
0x10,0x30,0x7c,0x30,0x30,0x34,0x18,0x00, // t
0x00,0x00,0xcc,0xcc,0xcc,0xcc,0x76,0x00, // u
0x00,0x00,0xcc,0xcc,0xcc,0x78,0x30,0x00, // v
0x00,0x00,0xc6,0xd6,0xfe,0xfe,0x6c,0x00, // w
0x00,0x00,0xc6,0x6c,0x38,0x6c,0xc6,0x00, // x
0x00,0x00,0xcc,0xcc,0xcc,0x7c,0x0c,0xf8, // y
0x00,0x00,0xfc,0x98,0x30,0x64,0xfc,0x00, // z
0x1c,0x30,0x30,0xe0,0x30,0x30,0x1c,0x00, // {
0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00, // |
0xe0,0x30,0x30,0x1c,0x30,0x30,0xe0,0x00, // }
0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00, // ~
0x80,0x0b,0x00,0x20,0x01,0x01,0x00,0x08 // 
};

const   char     string8X16Code[]= {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/

    0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x00,0x00,0x38,0x38,0x00,0x00,/*"!",1*/

    0x00,0x36,0x7E,0x6C,0xD8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*""",2*/

    0x00,0x00,0x00,0x6C,0x6C,0x6C,0xFE,0xD8,0xD8,0xD8,0xFE,0xD8,0xD8,0xD8,0x00,0x00,/*"#",3*/

    0x00,0x00,0x30,0x78,0xFC,0xFC,0xF0,0x70,0x38,0x3C,0x3C,0xFC,0xFC,0x78,0x30,0x30,/*"$",4*/

    0x00,0x00,0x00,0xCC,0xEC,0xF8,0xF8,0xF8,0xFC,0x3E,0x7E,0x7E,0x7E,0xCC,0x00,0x00,/*"%",5*/

    0x00,0x00,0x00,0x70,0xD8,0xD8,0xD8,0xF0,0xFE,0xEC,0xBC,0x98,0x9B,0xFE,0x00,0x00,/*"&",6*/

    0x00,0xE0,0xE0,0x60,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"'",7*/

    0x00,0x06,0x0C,0x18,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0x18,0x18,0x0C,0x06,0x00,/*"(",8*/

    0x00,0xC0,0x60,0x30,0x30,0x18,0x18,0x18,0x18,0x18,0x18,0x30,0x30,0x60,0xC0,0x00,/*")",9*/

    0x00,0x00,0x00,0x00,0x30,0x30,0xFE,0x78,0x78,0xFE,0x30,0x30,0x00,0x00,0x00,0x00,/*"*",10*/

    0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0xFE,0x30,0x30,0x30,0x30,0x00,0x00,0x00,/*"+",11*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xE0,0x60,0xC0,/*",",12*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"-",13*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xE0,0x00,0x00,/*".",14*/

    0x00,0x00,0x03,0x06,0x06,0x0C,0x0C,0x18,0x18,0x30,0x30,0x60,0x60,0xC0,0xC0,0x00,/*"/",15*/

    0x00,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00,/*"0",16*/

    0x00,0x00,0x00,0x30,0xF0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xFC,0x00,0x00,/*"1",17*/

    0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x0C,0x0C,0x18,0x30,0x60,0xC6,0xFE,0x00,0x00,/*"2",18*/

    0x00,0x00,0x00,0x7C,0xC6,0xC6,0x0C,0x38,0x0C,0x06,0x06,0xC6,0xCC,0x78,0x00,0x00,/*"3",19*/

    0x00,0x00,0x00,0x0C,0x1C,0x3C,0x6C,0x6C,0xCC,0xCC,0xFE,0x0C,0x0C,0x3E,0x00,0x00,/*"4",20*/

    0x00,0x00,0x00,0xFE,0xC0,0xC0,0xC0,0xF8,0xEC,0x06,0x06,0xC6,0xCC,0x78,0x00,0x00,/*"5",21*/

    0x00,0x00,0x00,0x3C,0x6C,0xC0,0xC0,0xF8,0xEC,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00,/*"6",22*/

    0x00,0x00,0x00,0xFE,0xCC,0xCC,0x18,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0x00,0x00,/*"7",23*/

    0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x6C,0x38,0x6C,0xC6,0xC6,0xC6,0x7C,0x00,0x00,/*"8",24*/

    0x00,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0x6E,0x3E,0x06,0x06,0x6C,0x78,0x00,0x00,/*"9",25*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x00,0x00,0x38,0x38,0x00,0x00,/*":",26*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x60,/*";",27*/

    0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0xC0,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,/*"<",28*/

    0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,/*"=",29*/

    0x00,0x00,0x00,0xC0,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0xC0,0x00,0x00,/*">",30*/

    0x00,0x00,0x00,0x7C,0xC6,0xC6,0xE6,0x06,0x0C,0x18,0x18,0x00,0x38,0x38,0x00,0x00,/*"?",31*/

    0x00,0x00,0x00,0x78,0xCC,0xFE,0xFE,0xFE,0xFE,0xFE,0xFC,0xC6,0xCC,0x78,0x00,0x00,/*"@",32*/

    0x00,0x00,0x00,0x30,0x30,0x38,0x78,0x78,0x6C,0x7C,0xCC,0xC6,0xC6,0xEF,0x00,0x00,/*"A",33*/

    0x00,0x00,0x00,0xF8,0xCC,0xCC,0xCC,0xF8,0xCC,0xC6,0xC6,0xC6,0xCC,0xF8,0x00,0x00,/*"B",34*/

    0x00,0x00,0x00,0x7E,0xC6,0xC6,0x80,0x80,0x80,0x80,0x80,0xC6,0xCC,0x78,0x00,0x00,/*"C",35*/

    0x00,0x00,0x00,0xF8,0xCC,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xCC,0xF8,0x00,0x00,/*"D",36*/

    0x00,0x00,0x00,0xFC,0xC6,0xD8,0xD8,0xF8,0xD8,0xD8,0xC0,0xC6,0xC6,0xFC,0x00,0x00,/*"E",37*/

    0x00,0x00,0x00,0xFC,0xC6,0xD8,0xD8,0xF8,0xD8,0xD8,0xC0,0xC0,0xC0,0xE0,0x00,0x00,/*"F",38*/

    0x00,0x00,0x00,0x7C,0xCC,0xCC,0x80,0x80,0x80,0x9E,0x8C,0xCC,0xCC,0x78,0x00,0x00,/*"G",39*/

    0x00,0x00,0x00,0xEF,0xC6,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xEF,0x00,0x00,/*"H",40*/

    0x00,0x00,0x00,0xFC,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xFC,0x00,0x00,/*"I",41*/

    0x00,0x00,0x00,0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x98,0xF0,/*"J",42*/

    0x00,0x00,0x00,0xFE,0xCC,0xD8,0xF0,0xF0,0xF0,0xD8,0xD8,0xCC,0xCC,0xFE,0x00,0x00,/*"K",43*/

    0x00,0x00,0x00,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC6,0xFE,0x00,0x00,/*"L",44*/

    0x00,0x00,0x00,0xFE,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFE,0x00,0x00,/*"M",45*/

    0x00,0x00,0x00,0xCF,0xE6,0xE6,0xF6,0xF6,0xDE,0xDE,0xDE,0xCE,0xCE,0xE6,0x00,0x00,/*"N",46*/

    0x00,0x00,0x00,0x78,0xCC,0x86,0x86,0x86,0x86,0x86,0x86,0x86,0xCC,0x78,0x00,0x00,/*"O",47*/

    0x00,0x00,0x00,0xFC,0xC6,0xC6,0xC6,0xC6,0xFC,0xC0,0xC0,0xC0,0xC0,0xE0,0x00,0x00,/*"P",48*/

    0x00,0x00,0x00,0x78,0xCC,0x86,0x86,0x86,0x86,0x86,0xF6,0xDE,0xDC,0x78,0x0E,0x00,/*"Q",49*/

    0x00,0x00,0x00,0xFC,0xC6,0xC6,0xC6,0xFC,0xD8,0xD8,0xCC,0xCC,0xC6,0xE7,0x00,0x00,/*"R",50*/

    0x00,0x00,0x00,0x7E,0xC6,0xC6,0xC0,0x60,0x38,0x0C,0x06,0xC6,0xC6,0xFC,0x00,0x00,/*"S",51*/

    0x00,0x00,0x00,0xFE,0xB6,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x00,/*"T",52*/

    0x00,0x00,0x00,0xEF,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,/*"U",53*/

    0x00,0x00,0x00,0xEF,0xC6,0xC6,0xCC,0x6C,0x6C,0x78,0x78,0x38,0x30,0x30,0x00,0x00,/*"V",54*/

    0x00,0x00,0x00,0xFE,0xB6,0xB6,0xB6,0xB6,0xFE,0xFE,0xFC,0xCC,0xCC,0xCC,0x00,0x00,/*"W",55*/

    0x00,0x00,0x00,0xEF,0xC6,0x6C,0x6C,0x38,0x38,0x38,0x6C,0x6C,0xC6,0xEF,0x00,0x00,/*"X",56*/

    0x00,0x00,0x00,0xFE,0xCC,0xCC,0x78,0x78,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x00,/*"Y",57*/

    0x00,0x00,0x00,0xFE,0x8C,0x0C,0x18,0x18,0x30,0x60,0x60,0xC6,0xC6,0xFC,0x00,0x00,/*"Z",58*/

    0x00,0x3E,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3E,0x00,/*"[",59*/

    0x00,0x00,0xC0,0xC0,0x60,0x60,0x30,0x30,0x30,0x18,0x18,0x0C,0x0C,0x0C,0x06,0x06,/*"\",60*/

    0x00,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xF8,0x00,/*"]",61*/

    0x00,0x3C,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"^",62*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,/*"_",63*/

    0x00,0xE0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"`",64*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0x3E,0x66,0xC6,0xC6,0x7F,0x00,0x00,/*"a",65*/

    0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xF8,0xEC,0xC6,0xC6,0xC6,0xEC,0xF8,0x00,0x00,/*"b",66*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x66,0xC0,0xC0,0xC0,0x66,0x3C,0x00,0x00,/*"c",67*/

    0x00,0x00,0x00,0x0E,0x06,0x06,0x06,0x3E,0x66,0xC6,0xC6,0xC6,0x6E,0x3F,0x00,0x00,/*"d",68*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,/*"e",69*/

    0x00,0x00,0x00,0x1F,0x33,0x30,0x30,0xFE,0x30,0x30,0x30,0x30,0x30,0xFC,0x00,0x00,/*"f",70*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0xCC,0xCC,0x78,0xC0,0x7C,0xC6,0xC6,0x7C,/*"g",71*/

    0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xFC,0xE6,0xC6,0xC6,0xC6,0xC6,0xEF,0x00,0x00,/*"h",72*/

    0x00,0x00,0x00,0x70,0x70,0x00,0x00,0xF0,0x30,0x30,0x30,0x30,0x30,0xFC,0x00,0x00,/*"i",73*/

    0x00,0x00,0x00,0x1C,0x1C,0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0xCC,0xF8,/*"j",74*/

    0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xDE,0xD8,0xF0,0xF8,0xD8,0xCC,0xFE,0x00,0x00,/*"k",75*/

    0x00,0x00,0x00,0xF0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xFC,0x00,0x00,/*"l",76*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xDB,0xDB,0xDB,0xDB,0xDB,0xFF,0x00,0x00,/*"m",77*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xE6,0xC6,0xC6,0xC6,0xC6,0xEF,0x00,0x00,/*"n",78*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,/*"o",79*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xEC,0xC6,0xC6,0xC6,0xCC,0xF8,0xC0,0xE0,/*"p",80*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x66,0xC6,0xC6,0xC6,0x66,0x3E,0x06,0x0F,/*"q",81*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x76,0x60,0x60,0x60,0x60,0xF8,0x00,0x00,/*"r",82*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0xC6,0xC0,0x7C,0x06,0xC6,0xFC,0x00,0x00,/*"s",83*/

    0x00,0x00,0x00,0x00,0x00,0x30,0x30,0xFC,0x30,0x30,0x30,0x30,0x30,0x1C,0x00,0x00,/*"t",84*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCE,0xC6,0xC6,0xC6,0xC6,0xCE,0x7F,0x00,0x00,/*"u",85*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0xC6,0x6C,0x6C,0x78,0x30,0x30,0x00,0x00,/*"v",86*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xB6,0xB6,0xFE,0xFE,0xCC,0xCC,0x00,0x00,/*"w",87*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x6C,0x38,0x38,0x38,0x6C,0xFE,0x00,0x00,/*"x",88*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF,0xC6,0x6C,0x6C,0x78,0x38,0x30,0x30,0xE0,/*"y",89*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xCC,0x18,0x30,0x30,0x66,0xFE,0x00,0x00,/*"z",90*/

    0x00,0x07,0x0C,0x0C,0x0C,0x0C,0x0C,0x18,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x07,0x00,/*"{",91*/

    0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,/*"|",92*/

    0x00,0xE0,0x30,0x30,0x30,0x30,0x30,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0xE0,0x00,/*"}",93*/

    0x71,0xDD,0xC7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00/*"~",94*/
};



